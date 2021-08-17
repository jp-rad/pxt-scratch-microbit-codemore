#include "pxt.h"

#if MICROBIT_CODAL
#include "MbitMoreServiceV2.h"
#else
#include "MbitMoreService.h"
#endif

#define UPDATE_PERIOD 11
#define NOTIFY_PERIOD 101

//% color=#FF9900 weight=95 icon="\uf1b0"
namespace MbitMore {
    MbitMoreService* _pService = NULL;
    MicroBitEvent _evt(DEVICE_ID_CODEMORE, DEVICE_CODEMORE_EVT_DIPSPLAY_TEXT_CMD);

    ManagedString _lastDisplayTextCommand("");

    /**
     * Get last display text command
     */
    //% block
    //% deprecated=true blockHidden=1 
    String getLastDisplayTextCommand() {
        return PSTR(_lastDisplayTextCommand);
    }

    int	displayTextCommand(MicroBit &e, ManagedString &mstr) {
        _lastDisplayTextCommand = mstr;
        _evt.fire();
        return 0; // handled!
    }

    void update() {
        while (NULL != _pService) {
            _pService->update();
            fiber_sleep(UPDATE_PERIOD);
        }
    }

    void notifyScratch() {
        while (NULL != _pService) {
            // notyfy data to Scratch
            _pService->notify();
            fiber_sleep(NOTIFY_PERIOD);
        }
    }

    /**
    * Starts a Scratch extension service.
    */
    //%
    void startMbitMoreService() {
        if (NULL != _pService) return;

        _pService = new MbitMoreService(uBit);
        _pService->assigneCallbackDisplayTextCommand(displayTextCommand);
        create_fiber(update);
        create_fiber(notifyScratch);
    }
    
    /**
     * Used internally by the library.
     */
    //% block
    //% deprecated=true blockHidden=1 
    void onCodemoreEventImpl(Action body) {
        //registerWithDal(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, body);
        registerWithDal(DEVICE_ID_CODEMORE, DEVICE_CODEMORE_EVT_DIPSPLAY_TEXT_CMD, body);
    }

}
