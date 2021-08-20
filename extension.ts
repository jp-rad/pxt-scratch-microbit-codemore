/**
 * CodeMore
 */
//% color=#0082FB weight=96 icon="\uf294"
namespace CodeMore {

    /**
    * Starts BLE services for Scratch Microbit-More extension.
    */
    //% blockId=CodeMore_startCodeMoreService block="start Microbit-More service"
    //% shim=CodeMore::startService
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
    //% shim=CodeMore::onCodemoreEvent
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
    //% blockId=CodeMore_onDisplayTextCommand block="on display text command received" blockGap=16
    //% draggableParameters=reporter
    export function onDisplayTextCommand(body: (cmd: string) => void) {
        init();
        onDisplayTextCommandBody = body;
    }

}