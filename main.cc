/*
 * AISDI BST Map Tester  v0.4  by mina86 <mina86@tlen.pl>
 * Licensed under the Academic Free License version 3.0
 */

#ifndef MINA_DEBUG
# define MINA_DEBUG 0
#endif
#if MINA_DEBUG
# define cerr cout
#endif

/*
 * If you did not implement const_iterator::operator--() set
 * TEST_REVERSE_ITERATORS and SPEED_RITER_ITERATIONS to 0.
 */

/*
 * If this test is used to test or benchmark unordered map (for
 * instance hash table) set ERASE_RANGE_ITERATIONS to 0 since this
 * test does not support unordered maps as far as erasing ranges is
 * concerned.
 */

/*
 * TODO: In sorted maps check if iterators give elements in correct
 * order.
 */

/* Do not touch this part */
#if defined DO_TEST && ! defined DO_SPEED_TEST
# define DO_SPEED_TEST           (!DO_TEST)
#endif
#if defined DO_SPEED_TEST && ! defined DO_TEST
# define DO_TEST                 (!DO_SPEED_TEST)
#endif

/* Here you can configure if you like */
#ifndef DO_TEST
# define DO_TEST                 1
#endif
#ifndef DO_SPEED_TEST
# define DO_SPEED_TEST           0
#endif

#if DO_TEST
# define TEST_REVERSE_ITERATORS  1
# define KEY_MAX                 4096
# define EMPTY_ERASE_ITERATIONS  (KEY_MAX / 16)
# define INS_ITERATIONS          (KEY_MAX / 2)
# define SET_ITERATIONS          (KEY_MAX / 2)
# define ERASE_ITERATIONS        (INS_ITERATIONS / 4)
# define ERASE_BY_KEY_ITERATIONS ERASE_ITERATIONS
# define ERASE_RANGE_ITERATIONS  (ERASE_ITERATIONS / 4)
# define ERASE_RANGE_MAX         16
# define FIND_ITERATIONS         KEY_MAX

# define MAPS_COUNT              4
# define SRAND_RANDOMIZE         0
# define SRAND_SEED              0
#endif

#if DO_SPEED_TEST
# define SPEED_KEY_MAX           0
# define SPEED_INS_ITERATIONS    (1 << 19)
# define SPEED_FIND_ITERATIONS   (1 << 19)
# define SPEED_ERASE_ITERATIONS  (1 << 19)
# define SPEED_ITER_ITERATIONS   (1 <<  3)
# define SPEED_RITER_ITERATIONS  (1 <<  3)

# define SPEED_SRAND_RANDOMIZE   0
# define SPEED_SRAND_SEED        0
#endif


#include <iostream>
#include <iomanip>
#include <map>
#include "TreeMap.h"
#include <stdlib.h>
#if SRAND_RANDOMIZE || SPEED_SRAND_RANDOMIZE
# include <time.h>
#endif
#if DO_SPEED_TEST
# include <string.h>
#endif

unsigned long error_counter = 0;

/******************** Timer ********************/
#if DO_SPEED_TEST
#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>


double timer() {
	static unsigned int old_s, old_u;
	unsigned int new_s, new_u, diff_s, diff_u;
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);

	new_s = usage.ru_utime.tv_sec + usage.ru_stime.tv_sec;
	new_u = usage.ru_utime.tv_usec + usage.ru_stime.tv_usec;
	if (new_u > 1000000) {
		++new_s;
		new_u -= 1000000;
	}

	diff_s = new_s - old_s;
	if (old_u <= new_u) {
		diff_u = new_u - old_u;
	} else {
		/* getrusage wywol. w krotkim odstepie czasu najwyrazniej
		   czasem "glupieje", bo na Solarisie (i tylko tu) bez tego
		   sprawdzenia dostawalem czasem czasy ujemne */
		if (diff_s) --diff_s;
		diff_u = 1000000 + new_u - old_u;
	}

	old_s = new_s;
	old_u = new_u;
	return diff_s + diff_u/(double)1000000.0;
}
#else
#include <windows.h>
LARGE_INTEGER freq;
LARGE_INTEGER last_time;
LARGE_INTEGER now_time;
double timer()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&now_time);
	double deltaT = (double)(now_time.QuadPart - last_time.QuadPart) / (double)freq.QuadPart;
	last_time = now_time;
	return deltaT;
}
#endif
#endif

int CCount::count = 0;



/******************** If RAND_MAX is too small ********************/
#define BIG_RAND      (((long)RAND_MAX + 1) * rand()   + rand()  )
#define BIG_RAND_MAX  (((long)RAND_MAX + 1) * RAND_MAX + RAND_MAX)



/******************** Types ********************/
typedef TreeMap MyMap;
typedef MyMap::Key Key;
typedef MyMap::Val Val;
#if !SPEED_TEST
typedef std::map<Key, Val> StdMap;
typedef std::pair<MyMap, StdMap> MapPair;
typedef std::pair<Key, Val> Entry;
typedef std::pair<MyMap::iterator, bool> MyInsRet;
typedef std::pair<StdMap::iterator, bool> StdInsRet;
#endif



#if DO_TEST
/******************** Output formaters ********************/
std::ostream &operator<<(std::ostream &os, const Entry &entry) {
	return os << '(' << std::setw(5) << entry.first << ", " << entry.second << ')';
}



/******************** Finds elements in maps ********************/
static std::pair<MyMap::iterator, StdMap::iterator>
find(MyMap &m, StdMap &sm, const Key &k) {
	/* Find key */
	MyMap::iterator it = m.find(k);
	if (it!=m.end() && it->first!=k) {
		std::cerr << "err: searched for " << std::setw(5) << k << " but got "
		          << *it << std::endl;
		++error_counter;
	}

	/* If found test with [] */
	if (it!=m.end() && it->second!=m[k]) {
		std::cerr << "err: value of find(" << std::setw(5) << k << ") ("
		          << it->first << ") differs from m[" << std::setw(5) << k
		          << "] (" << m[k] << ')' << std::endl;
		++error_counter;
	}

	/* Compare with StdMap */
	StdMap::iterator sit = sm.find(k);
	if (sit==sm.end()) {
		if (it!=m.end()) {
			std::cerr << "err: key " << std::setw(5) << k
			          << " does not exist in StdMap but exists in MyMap; "
			          << *it << std::endl;
			++error_counter;
		}
	} else if (it==m.end()) {
		std::cerr << "err: key " << std::setw(5) << k
		          << " exists in StdMap but does not exist in MyMap; "
		          << *sit << std::endl;
		++error_counter;
	} else if (it->first!=sit->first || it->second!=sit->second) {
		std::cerr << "err: StdMap contains " << *sit << " but MyMap contains "
		          << *it << std::endl;
		++error_counter;
	}

	return std::make_pair(it, sit);
}



/********** Checks number of elements **********/
static void elements(MyMap &m, StdMap &sm, bool print = true) {
	if (m.size()!=sm.size()) {
		std::cerr << "err: Number of elements " << m.size()
		          << " should be " << sm.size() << std::endl;
		++error_counter;
	} else if (print){
		std::cout << "Number of elements " << m.size() << std::endl;
	}
	std::cout << std::endl;
}



/********** Checks elements count **********/
static void count_changed(size_t from, size_t to, size_t expected) {
	if (to==expected) {
		return;
	}
	if (from==to) {
		std::cerr << "err: elements count stayed at " << from
		          << " where should change to " << expected << std::endl;
		++error_counter;
		return;
	}

	std::cerr << "err: elements count changed from " << from << " to " << to;
	++error_counter;
	if (from==expected) {
		std::cerr << " where should stayed at " << expected << std::endl;
	} else {
		std::cerr << " where should change to " << expected << std::endl;
	}
}



/********** Inserts entry **********/
static void insert(MyMap &m, StdMap &sm, const Key &k, const Val &v) {
	const Entry entry = std::make_pair(k, v);
	std::cout << ":: inserting " << entry << std::endl;
	MyMap::iterator it = find(m, sm, k).first;
	bool found = it != m.end();
	const Entry tmp = found ? *it : entry;

	size_t num = m.size();
	MyInsRet ret = m.insert(entry);
	sm.insert(entry);

	if (found == ret.second) {
		std::cerr << "err: insertion of " << entry
		          << (ret.second ? " succeed but should fail"
		                         : " failed but should succeed") << std::endl;
		++error_counter;
	}
	if (ret.first->first!=k) {
		std::cerr << "err: inserted " << std::setw(5) << k << " but got "
		          << *ret.first << std::endl;
		++error_counter;
	}
	if (!found && ret.first->second!=v) {
		std::cerr << "err: inserted " << v << " but got " << *ret.first << std::endl;
		++error_counter;
	}
	if (found && *ret.first!=tmp) {
		std::cerr << "err: value has changed but it shouldn't" << std::endl;
		++error_counter;
	}

	count_changed(num, m.size(), num + !found);
}



/********** Sets entry **********/
static void set(MyMap &m, StdMap &sm, const Key &k, const Val &v) {
	std::cout << ":: setting " << std::make_pair(k, v) << std::endl;
	bool found = find(m, sm, k).first != m.end();;

	size_t num = m.size();
	m [k] = v;
	sm[k] = v;
	MyMap::iterator it = find(m, sm, k).first;

	if (it==m.end()) {
		std::cerr << "err: set " << std::setw(5) << k
		          << " but find() returns {end}" << std::endl;
		++error_counter;
	} else {
		if (it->first!=k) {
			std::cerr << "err: set " << std::setw(5) << k
			          << " but got " << *it << std::endl;
			++error_counter;
		}
		if (it->second!=v) {
			std::cerr << "err: set " << std::setw(5) << v
			          << " but got " << *it << std::endl;
			++error_counter;
		}
	}

	count_changed(num, m.size(), num + !found);
}



/********** Erases entry **********/
static void erase(MyMap &m, StdMap &sm, const Key &k) {
	std::cout << ":: erasing " << std::setw(5) << k << std::endl;
	const std::pair<MyMap::iterator, StdMap::iterator> p = find(m, sm, k);
	MyMap::iterator it = p.first;
	StdMap::iterator sit = p.second;

	size_t num = m.size();
	bool found = it != m.end();
	if (found) {
		MyMap::iterator i2 = m.erase(it);
		sm.erase(sit);
	}

	count_changed(num, m.size(), num - found);
}



/********** Erases entry by key **********/
static void erase_by_key(MyMap &m, StdMap &sm, const MyMap::Key &k) {
	std::cout << ":: erasing (by key) " << std::setw(5) << k << std::endl;
	const std::pair<MyMap::iterator, StdMap::iterator> p = find(m, sm, k);
	MyMap::iterator it = p.first;
	StdMap::iterator sit = p.second;
	bool found = it!=m.end();

	size_t num = m.size();
	int removed = m.erase(k);
	sm.erase(k);

	if (found && !removed) {
		std::cerr << "err: erase removed nothing but element was found" << std::endl;
		++error_counter;
	} else if (!found && removed) {
		std::cerr << "err: erase removed something but nothing was found" << std::endl;
		++error_counter;
	}

	count_changed(num, m.size(), num - (it!=m.end()));
}



/********** Erases number of entries **********/
#if ERASE_RANGE_ITERATIONS
static void erase_range(MyMap &m, StdMap &sm, const Key &k, size_t range) {
	std::cout << ":: erasing " << range << " key(s) from "
	          << std::setw(5) << k << std::endl;
	const std::pair<MyMap::iterator, StdMap::iterator> p = find(m, sm, k);
	MyMap::iterator it = p.first;
	StdMap::iterator sit = p.second;

	MyMap::iterator end = it;
	StdMap::iterator send = sit;
	size_t rem = 0;
	for (size_t i = range; i; --i) {
		if (end!=m.end()) {
			++end;
			++rem;
		}
		if (send!=sm.end()) ++send;
	}

	size_t num = m.size();
	bool end_is_end = end == m.end();
	Entry end_entry = end_is_end ? Entry() : *end;
	MyMap::iterator ret = m.erase(it, end);
	sm.erase(sit, send);

	if (( end_is_end && ret!=m.end()) ||
	    (!end_is_end && ret==m.end()) ||
	    (!end_is_end && *ret!=end_entry)) {
		std::cerr << "err: erase returned ";
		++error_counter;
		if (ret==m.end()) {
			std::cerr << "{end}";
		} else {
			std::cerr << '{' << *ret << '}';
		}
		std::cerr << " where ";
		if (end_is_end) {
			std::cerr << "{end}";
		} else {
			std::cerr << '{' << end_entry << '}';
		}
		std::cerr << " expected" << std::endl;
	}

	count_changed(num, m.size(), num - rem);
}
#endif



/********** Tests iterators **********/
static void iterators(MyMap &m, StdMap sm) {
	MyMap::size_type size = m.size(), num = 0;
	MyMap::iterator it = m.begin(), end = m.end();

	while (it!=end) {
		const MyMap::P &entry = *it;
		const StdMap::iterator sit = sm.find(entry.first);

		std::cout << ":: iterating through " << entry << std::endl;
		if (sit==sm.end()) {
			std::cerr << "We were in " << entry << " already!" << std::endl;
			++error_counter;
		} else {
			sm.erase(sit);
		}

		++it;
		++num;
	}

	if (num!=size) {
		std::cerr << "Map contains " << size << " elements but we where in "
		          << num << std::endl;
		++error_counter;
	}
}



/********** Test reverse iterators **********/
#if TEST_REVERSE_ITERATORS
static void reverse_iterators(MyMap &m, StdMap sm) {
	MyMap::size_type size = m.size(), num = 0;
	MyMap::iterator it = m.end(), begin = m.begin();

	while (it!=begin) {
		--it;
		++num;

		const MyMap::P &entry = *it;
		const StdMap::iterator sit = sm.find(entry.first);

		std::cout << ":: iterating in reverse through " << entry << std::endl;
		if (sit==sm.end()) {
			std::cerr << "We were in " << entry << " already!\n";
			++error_counter;
		} else {
			sm.erase(sit);
		}
	}

	if (num!=size) {
		std::cerr << "Map contains " << size << " elements but we where in "
		          << num << std::endl;
		++error_counter;
	}
}
#endif



/********** Compare two maps **********/
#if MAPS_COUNT > 1
static void compare(unsigned id1, unsigned id2, MyMap &m1, MyMap &m2,
                    bool result = false) {
	bool equal = m1.info_eq(m2);
	result = result || id1==id2;

	if (id1!=id2 && equal!=m2.info_eq(m1)) {
		std::cerr << '#' << id1 << (equal ? " == " : " != ") << '#' << id2
		          << " but #" << id2 << (equal ? " != " : " == ") << '#' << id1
		          << std::endl;
		++error_counter;
	}

	if (equal!=result) {
		std::cerr << '#' << id1 << (equal ? " == " : " != ") << '#' << id2
		          << " but expected otherwise" << std::endl;
		++error_counter;
	}

	equal = m1.struct_eq(m2);
	if (id1!=id2 && equal!=m2.struct_eq(m1)) {
		std::cerr << '#' << id1 << "has "
		          << (equal ? " the same " : " different ")
		          << "structure then #" << id2 << " but #" << id2 << "has"
		          << (equal ? " different " : " the same ")
		          << "structure then #" << id2 << std::endl;
		++error_counter;
	}
	if (id1==id2 && !equal) {
		std::cerr << '#' << id1 << " doesn't have the same structure as itself"
		          << std::endl;
		++error_counter;
	}

}
#endif


#undef RAND_KEY
#if KEY_MAX
# if KEY_MAX > (RAND_MAX << 2)
#  define RAND_KEY (rand()   % KEY_MAX - KEY_MAX/2)
# else
#  define RAND_KEY (BIG_RAND % KEY_MAX - KEY_MAX/2)
# endif
#else
# if RAND_MAX <= (1 << 16)
#  define RAND_KEY (BIG_RAND - BIG_RAND_MAX/2)
# else
#  define RAND_KEY (rand()   - RAND_MAX    /2)
# endif
#endif


/* Test function */
static void test(void) {
	/* Init */
	MapPair m[MAPS_COUNT];
	unsigned i, j, k;

#if SRAND_RANDOMIZE
	srand(time(0));
#else
	srand(SRAND_SEED);
#endif


	/* All should compare equal */
#if MAPS_COUNT > 1
	for (i = 0; i<(sizeof(m)/sizeof(*m)); ++i) {
		for (j = 0; j<(sizeof(m)/sizeof(*m)); ++j) {
			compare(i, j, m[i].first, m[j].first, true);
		}
	}
	std::cout << std::endl;
#endif


	/* Main test */
	for (k = 0; k<(sizeof(m)/sizeof(*m)); ++k) {
		std::cout << "=== Operates on map #" << k << " ===\n\n";

		/* Erase by key from empty map */
		for (i = 0; i<EMPTY_ERASE_ITERATIONS; ++i) {
			erase_by_key(m[k].first, m[k].second, RAND_KEY);
		}
		elements(m[k].first, m[k].second);

		/* Insert */
		for (i = 0; i<INS_ITERATIONS; ++i) {
			char str[9];
			for (j = 0; j<8; ++j) str[j] = 'A' + rand() % 26;
			str[j] = 0;
			insert(m[k].first, m[k].second, RAND_KEY, str);
		}
		elements(m[k].first, m[k].second);

		/* Set */
		for (i = 0; i<INS_ITERATIONS; ++i) {
			char str[9];
			for (j = 0; j<8; ++j) str[j] = 'A' + rand() % 26;
			str[j] = 0;
			set(m[k].first, m[k].second, RAND_KEY, str);
		}
		elements(m[k].first, m[k].second);

		/* Erase */
		for (i = 0; i<ERASE_ITERATIONS; ++i) {
			erase(m[k].first, m[k].second, RAND_KEY);
		}
		elements(m[k].first, m[k].second);

		/* Erase by key */
		for (i = 0; i<ERASE_BY_KEY_ITERATIONS; ++i) {
			erase_by_key(m[k].first, m[k].second, RAND_KEY);
		}
		elements(m[k].first, m[k].second);

#if ERASE_RANGE_ITERATIONS
		/* Erase range */
		for (i = 0; i<ERASE_RANGE_ITERATIONS; ++i) {
			erase_range(m[k].first, m[k].second, RAND_KEY,
			            rand() % ERASE_RANGE_MAX);
		}
		elements(m[k].first, m[k].second);
#endif

		/* Find */
#if FIND_ITERATIONS >= KEY_MAX
		for (i = -KEY_MAX/2; i < KEY_MAX/2; ++i) {
			find(m[k].first, m[k].second, i);
		}
#else
		for (i = 0; i < FIND_ITERATIONS; ++i) {
			find(m[k].first, m[k].second, RAND_KEY);
		}
#endif

		/* Iterators */
		iterators(m[k].first, m[k].second);
#if TEST_REVERSE_ITERATORS
		reverse_iterators(m[k].first, m[k].second);
#endif

#if ERASE_RANGE_ITERATIONS
		/* Erase range till there's nothing left */
		while (m[k].first.size() > 128) {
			unsigned num = m[k].first.size();
			num = num + (unsigned)(num * (rand() / (RAND_MAX + 1.0)));
			num = num >> 1;
			erase_range(m[k].first, m[k].second, RAND_KEY, num >> 1);
		}
		erase_range(m[k].first, m[k].second,
		            m[k].first.begin()->first, m[k].first.size());
		elements(m[k].first, m[k].second);
#endif
	}


#if MAPS_COUNT > 1
	/* Compare */
	for (i = 0; i<(sizeof(m)/sizeof(*m)); ++i) {
		for (j = 0; j<(sizeof(m)/sizeof(*m)); ++j) {
			compare(i, j, m[i].first, m[j].first, i==j || m[i].second==m[j].second);
		}
	}
	std::cout << std::endl;


	/* Create two equal maps */
	std::cout << "=== Creating two equal maps ===\n\n";
	m[0].first.clear(); m[0].second.clear();
	m[1].first.clear(); m[1].second.clear();

	compare(0, 0, m[0].first, m[0].first, true);
	compare(0, 1, m[0].first, m[1].first, true);
	compare(1, 0, m[1].first, m[0].first, true);
	compare(1, 1, m[1].first, m[1].first, true);

	/* Insert */
	for (i = 0; i<INS_ITERATIONS; ++i) {
		MyMap::Key key = RAND_KEY;
		char str[9];
		for (j = 0; j<8; ++j) str[j] = 'A' + rand() % 26;
		str[j] = 0;
		insert(m[0].first, m[0].second, key, str);
		insert(m[1].first, m[1].second, key, str);
	}
	elements(m[0].first, m[0].second);
	elements(m[1].first, m[1].second);

	/* Set */
	for (i = 0; i<INS_ITERATIONS; ++i) {
		MyMap::Key key = RAND_KEY;
		char str[9];
		for (j = 0; j<8; ++j) str[j] = 'A' + rand() % 26;
		str[j] = 0;
		set(m[0].first, m[0].second, key, str);
		set(m[1].first, m[1].second, key, str);
	}
	elements(m[0].first, m[0].second);
	elements(m[1].first, m[1].second);

	/* Compare */
	compare(0, 0, m[0].first, m[0].first, true);
	compare(0, 1, m[0].first, m[1].first, true);
	compare(1, 0, m[1].first, m[0].first, true);
	compare(1, 1, m[1].first, m[1].first, true);
#endif

	/* Copy constructor test */
	std::cout << "=== Cloning #0 to #666 ===\n";
	MyMap *clone = new MyMap(m[0].first);
	compare(0, 666, m[0].first, *clone, true);
	compare(0, 666, *clone, m[0].first, true);
	delete clone;
}
#endif /* DO_TEST */



/* Speed test */
#if DO_SPEED_TEST


#undef RAND_KEY
#if SPEED_KEY_MAX
# if SPEED_KEY_MAX > (RAND_MAX << 2)
#  define RAND_KEY (rand()   % SPEED_KEY_MAX - SPEED_KEY_MAX/2)
# else
#  define RAND_KEY (BIG_RAND % SPEED_KEY_MAX - SPEED_KEY_MAX/2)
# endif
#else
# if RAND_MAX <= (1 << 16)
#  define RAND_KEY (BIG_RAND - BIG_RAND_MAX/2)
# else
#  define RAND_KEY (rand()   - RAND_MAX    /2)
# endif
#endif


#define PRINT_TIME(str) do {							\
		double _time = timer();							\
		std::cout.width(20);							\
		std::cout << std::left << (str) << ": ";		\
		std::cout.precision(8);							\
		std::cout << std::showpoint <<_time << " s\n";	\
	} while (0)


static void speed_test() {
	/* Init */
	MyMap m;
	unsigned i;

#if SPEED_SRAND_RANDOMIZE
	srand(time(0));
#else
	srand(SPEED_SRAND_SEED);
#endif

	/* Insert */
	timer();
	for (i = 0; i<SPEED_INS_ITERATIONS; ++i) {
		m.insert(std::make_pair(RAND_KEY, ""));
	}
	PRINT_TIME("Insert");

	/* Find */
	for (i = 0; i<SPEED_FIND_ITERATIONS; ++i) {
		m.find(RAND_KEY);
	}
	PRINT_TIME("Find");

	/* Erase */
	for (i = 0; i<SPEED_ERASE_ITERATIONS; ++i) {
		m.erase(RAND_KEY);
	}
	PRINT_TIME("Erase");

	/* Clear */
	m.clear();
	PRINT_TIME("Clear");

	/* Insert */
	for (i = 0; i<SPEED_INS_ITERATIONS; ++i) {
		m.insert(std::make_pair(RAND_KEY, ""));
	}
	PRINT_TIME("Insert");

	/* Iterators */
	for (i = 0; i<SPEED_ITER_ITERATIONS; ++i) {
		const MyMap::const_iterator end = m.end();
		MyMap::const_iterator it = m.begin();
		for (; it!=end; ++it);
	}
	PRINT_TIME("Iterators");

#if SPEED_RITER_ITERATIONS
	/* Iterators */
	for (i = 0; i<SPEED_RITER_ITERATIONS; ++i) {
		const MyMap::const_iterator begin = m.begin();
		MyMap::const_iterator it = m.end();
		while (--it != begin);
	}
	PRINT_TIME("Reverse Iterators");
#endif

	/* Find + Erase */
	MyMap::iterator it;
	for (i = 0; i<SPEED_ERASE_ITERATIONS; ++i) {
		it=m.find(RAND_KEY);
		if (it != m.end())
			m.erase(it);
	}
	PRINT_TIME("Find + erase");

	/* Insert */
	for (i = 0; i<SPEED_INS_ITERATIONS; ++i) {
		m.insert(std::make_pair(RAND_KEY, ""));
	}
	PRINT_TIME("Insert");

#if ERASE_RANGE_ITERATIONS
	{
		/* Save keys */
		unsigned count = m.size();
		int *keys = new int[count];
		{
			const MyMap::const_iterator end = m.end();
			MyMap::const_iterator it = m.begin();
			for (int *k = keys; it!=end; ++it, ++k) *k = it->first;
		}

		/* Erase by range */
		unsigned first, last;
		timer();
		while (m.size()) {
			first = BIG_RAND % count;
			last = first + ((rand() / (RAND_MAX + 1.0)) * 0.7 + 0.3) * count;
			if (last>=count) last = count - 1;

			m.erase(m.find(keys[first]), ++(m.find(keys[last])));
			++last;
			memcpy(keys + first, keys + last, (count - last) * sizeof *keys);
			count -= last - first;
		}
		PRINT_TIME("Erase by range");

		delete[] keys;
	}
#endif

}
#endif /* DO_SPEED_TEST */



/* Main */
int main(void) {

#if DO_TEST
	test();
	if (CCount::getCount()) {
		std::cerr << "count = " << CCount::getCount()
		          << "; should be 0; memory leak\n";
		++error_counter;
	} else {
		std::cout << "count = " << CCount::getCount() << std::endl;
	}
	if (error_counter) {
		std::cerr << "Error count = " << error_counter
		          << "; should be 0; You have errors!\n";
		++error_counter;
	} else {
		std::cout << "Error count = " << error_counter << std::endl;
	}

#endif

#if DO_SPEED_TEST
	speed_test();
#endif
#ifdef WIN32
	system("pause");
#endif
	return 0;
}