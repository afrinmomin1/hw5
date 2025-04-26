#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
static bool backtrack(
    const AvailabilityMatrix& avail, // n x k matrix
    size_t dailyNeed, // workers needed per day
    size_t maxShifts, // max shifts per worker
    DailySchedule& sched, // current working schedule
    vector<int>& shiftCount, // running count of shifts per worker
    size_t pos // index into schedule slots
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    // number of days
    size_t n = avail.size();
    // number of workers
    size_t k = avail[0].size();

    // initialize empty schedule w/ n days with empty lists
    sched.clear();
    sched.resize(n);

    // track num of shifts assigned to each worker (start at 0)
    vector<int> shiftCount(k, 0);

    // start recursive backtracking from the first slot
    return backtrack(avail, dailyNeed, maxShifts, sched, shiftCount, 0);

}

// helper function recursive backtracking helper to fill sched
static bool backtrack(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, vector<int>& shiftCount, size_t pos) {
    // number of days
    size_t n = avail.size();
    // number of workers
    size_t k = avail[0].size();

    // base case: all n*d slots have been filled
    if(pos == n * dailyNeed) {
        return true;
    }

    // determine current day index
    size_t day = pos / dailyNeed;

    // try assigning each worker to the slot
    for(Worker_T w = 0; w < k; w++) {
        // check 1: is worker available this day?
        if(!avail[day][w]) {
            continue;
        }

        // check 2: has worker exceeded maxShifts?
        if(shiftCount[w] >= static_cast<int>(maxShifts)) {
            continue;
        }

        // check 3: is worker already scheduled this day?
        bool alreadyScheduled = false;
        for(Worker_T assigned : sched[day]) {
            if(assigned == w) {
                alreadyScheduled = true;
                break;
            }
        }
        if(alreadyScheduled) {
            continue;
        }

        // all checked passed: assign w to this slot
        sched[day].push_back(w);
        shiftCount[w]++;

        // recurse to fill the next slot
        if(backtrack(avail, dailyNeed, maxShifts, sched, shiftCount, pos + 1)) {
            // a completely valid schedule has been found
            return true;
        }

        // undo assignment (backtrack)
        shiftCount[w]--;
        sched[day].pop_back();
    }

    // no valid assignment for this slot
    return false;

}
