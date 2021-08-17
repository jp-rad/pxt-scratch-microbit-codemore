
namespace MbitMore {
    let sharedData: number[] = [0, 0, 0, 0];

    /**
    * Starts BLE services for Scratch Microbit-More extension.
    */
    //% blockId=MbitMore_startMbitMoreService block="start Microbit-More service"
    //% shim=MbitMore::startMbitMoreService
    export function startService():void {
        console.log("Microbit-More started");
    }

    let initialized = false;
    let onDisplayTextCommandBody: (cmd: string) => void;

    function init() {
        if (initialized) return;
        initialized = true;
        onCodemoreEvent(handleCodemoreEvent);
    }

    /**
     * Used internally by the library.
     */
    //% block
    //% deprecated=true blockHidden=1
    //% shim=MbitMore::onCodemoreEventImpl
    function onCodemoreEvent(body: () => void): void {
        console.log("shim=custom::onCodemoreEventImpl")
    }

    function handleCodemoreEvent(): void {
        if (onDisplayTextCommandBody) {
            let cmd: string = getLastDisplayTextCommand();
            onDisplayTextCommandBody(cmd);
        }
    }

    /**
     * on display text command received
     */
    //% blockId=MbitMore_onDisplayTextCommand block="on display text command received" blockGap=16
    //% draggableParameters=reporter
    export function onDisplayTextCommand(body: (cmd: string) => void) {
        init();
        onDisplayTextCommandBody = body;
    }

}