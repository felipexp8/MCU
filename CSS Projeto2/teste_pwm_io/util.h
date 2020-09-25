/*
 * util.h
 *
 *  Created on: 25/09/2020
 *      Author: Felipe
 */

#ifndef UTIL_H_
#define UTIL_H_

#define TRUE  1
#define FALSE 0
#define None 0x00

#define HOLDFOREVER() (while(TRUE){})

#define INICIARCLOCKS() (WDTCTL = WDTPW + WDTHOLD)

#endif /* UTIL_H_ */
