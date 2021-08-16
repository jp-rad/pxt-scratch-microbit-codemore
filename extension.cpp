#include "pxt.h"

#if MICROBIT_CODAL
#include "MbitMoreServiceV2.h"
#else
#include "MbitMoreService.h"
#endif

#define UPDATE_PERIOD 11
#define NOTIFY_PERIOD 101

enum SharedDataIndex {
    //% block="data0"
    DATA0 = 0,
    //% block="data1"
    DATA1 = 1,
    //% block="data2"
    DATA2 = 2,
    //% block="data3"
    DATA3 = 3,
};

//% color=#FF9900 weight=95 icon="\uf1b0"
namespace MbitMore {
    MbitMoreService* _pService = NULL;
    
    int	displayTextCommand(MicroBit &e, ManagedString &mstr) {
        uBit.display.stopAnimation();
        uBit.display.scrollAsync("CMD!", 120);
        return 0;   // handled!
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
    * Set shared data value.
    */
    //%
    void setMbitMoreSharedData(SharedDataIndex index, int value) {
        if (NULL == _pService) return;

        _pService->setSharedData((int)index, value);
    }

    /**
     * Get shared data value. 
     */
    //%
    int getMbitMoreSharedData(SharedDataIndex index) {
        if (NULL == _pService) return 0;

        return _pService->getSharedData((int)index);
    }    
}
