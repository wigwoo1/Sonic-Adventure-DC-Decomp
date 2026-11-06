#ifndef JP_NATIVE_ACTION_NUMERIC_H
#define JP_NATIVE_ACTION_NUMERIC_H
#ifndef JP_ACTION_NUMERIC_API
#define JP_ACTION_NUMERIC_API __declspec(dllexport)
#endif
/* Existing qualified SH-4 numerical value services,RZ/DN1. These bridge the
 * original action source's by-value FIPR and reciprocal-square-root calls.
 * They do not change host rounding mode or add a numerical/state owner. */
JP_ACTION_NUMERIC_API float jp_action_fipr_self(float,float,float,float);
JP_ACTION_NUMERIC_API float jp_action_fsrra(float);
#endif
