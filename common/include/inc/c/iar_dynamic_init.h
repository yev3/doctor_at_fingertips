/**************************************************
 *
 * Copyright 2010-2017 IAR Systems AB.
 *
 * Call the constructors of all global objects. This function is used to
 * manually initialize global objects when --skip_dynamic_initialization is
 * used.
 *
 **************************************************/

#ifndef IAR_DYNAMIC_INIT_H
#define IAR_DYNAMIC_INIT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Function to manually initiate dynamic initialization. 
 * When the linker has been instructed not to add dynamic initialization to the
 * system startup code using --skip_dynamic_initialization this function must
 * be used inside the application to trigger initialization.
 */
void __iar_dynamic_initialization(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IAR_DYNAMIC_INIT_H */

 
/*
  Local Variables: 
  mode: c
  fill-column: 79
  End:
*/
