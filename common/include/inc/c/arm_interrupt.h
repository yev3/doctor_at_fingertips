/********************************************************
 *
 * This file declares ARM interrupt vectors and handlers.
 *
 * Copyright 2000-2017 IAR Systems AB.
 *
 * $Revision: 112610 $
 *
 ********************************************************/

#ifndef __ARM_INTERRUPT_INCLUDED
#define __ARM_INTERRUPT_INCLUDED

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/* __iar_swi_handler handles swi requests and call the appropriate __swi */
/* function depending on the specified swi_number. __iar_swi_handler is  */
/* defined in swi_handler.s79 and is included in iar C libraries.        */
extern void __iar_swi_handler(void);

/* __next_swi_in_chain is used by __iar_swi_handler to chain different    */
/* swi_handlers. If __iar_swi_handler cannot handle the swi request       */
/* the function specified by __next_swi_in_chain is called.               */
extern unsigned int __next_swi_in_chain;


/* declare vector addresses for the different interrupt types */
unsigned int *resetvec = (unsigned int *)0x0;
unsigned int *irqvec   = (unsigned int *)0x18;
unsigned int *fiqvec   = (unsigned int *)0x1C;
unsigned int *swivec   = (unsigned int *)0x08;


#endif /* __ARM_INTERRUPT_INCLUDED */
