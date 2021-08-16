/*
MIT License

Copyright (c) 2020 jp-rad

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MBIT_CODE_MORE_H
#define MBIT_CODE_MORE_H

#include "pxt.h"
#include "MicroBit.h"

#ifndef MICROBIT_ID_CUSTOM
#define MICROBIT_ID_CUSTOM  9000
#endif

#ifndef DEVICE_ID_CODEMORE
#define DEVICE_ID_CODEMORE  (MICROBIT_ID_CUSTOM+1)
#endif

#ifndef DEVICE_CODEMORE_EVT_DIPSPLAY_TEXT_CMD
#define DEVICE_CODEMORE_EVT_DIPSPLAY_TEXT_CMD 1
#endif

typedef int	(*p_displayTextCommand)(MicroBit &, ManagedString &);

#endif
