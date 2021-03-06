#ifndef _TIMEBOMB_SM_H
#define _TIMEBOMB_SM_H

#include <assert.h>
#include <statemap.h>

struct TimeBomb;
struct timebombContext;

struct TimeBombState {
    void (*Entry)(struct timebombContext *const fsm);
    void (*Exit)(struct timebombContext *const fsm);
    void (*BUTTON)(struct timebombContext *const fsm, Event const *const e);
    void (*TIMEOUT)(struct timebombContext *const fsm, Event const *const e);
    void (*Default)(struct timebombContext *const fsm);
    STATE_MEMBERS
};

extern const struct TimeBombState TimeBombMap_wait4button;
extern const struct TimeBombState TimeBombMap_blink;
extern const struct TimeBombState TimeBombMap_pause;
extern const struct TimeBombState TimeBombMap_boom;

struct timebombContext {
    struct TimeBomb *_owner;
    FSM_MEMBERS(TimeBomb)
};

#ifdef NO_TIMEBOMB_SM_MACRO
extern void timebombContext_Init(struct timebombContext *const fsm, struct TimeBomb *const owner);
extern void timebombContext_EnterStartState(struct timebombContext *const fsm);
extern void timebombContext_BUTTON(struct timebombContext *const fsm, Event const *const e);
extern void timebombContext_TIMEOUT(struct timebombContext *const fsm, Event const *const e);
#else
#define timebombContext_Init(fsm, owner)       \
    FSM_INIT((fsm), &TimeBombMap_wait4button); \
    (fsm)->_owner = (owner)

#define ENTRY_STATE(state)      \
    if ((state)->Entry != NULL) {                           
        (state)->Entry(fsm);    \
    }

#define EXIT_STATE(state)      \
    if ((state)->Exit != NULL) {                       
        (state)->Exit(fsm);    \
    }

#define timebombContext_EnterStartState(fsm) \
    ENTRY_STATE(getState(fsm))

#define timebombContext_BUTTON(fsm, e) \
    assert(getState(fsm) != NULL);     \
    setTransition((fsm), "BUTTON");    \
    getState(fsm)->BUTTON((fsm), (e)); \
    setTransition((fsm), NULL)

#define timebombContext_TIMEOUT(fsm, e) \
    assert(getState(fsm) != NULL);      \
    setTransition((fsm), "TIMEOUT");    \
    getState(fsm)->TIMEOUT((fsm), (e)); \
    setTransition((fsm), NULL)
#endif

#endif