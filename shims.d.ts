// Auto-generated. Do not edit.



    //% color=#FF9900 weight=95 icon="\uf1b0"
declare namespace MbitMore {

    /**
     * Starts a Scratch extension service.
     * The handler can call ``setMbitMoreSharedData`` to send any data to Scratch.
     */
    //% shim=MbitMore::startMbitMoreService
    function startMbitMoreService(handler: () => void): void;

    /**
     * Set shared data value.
     */
    //% shim=MbitMore::setMbitMoreSharedData
    function setMbitMoreSharedData(index: SharedDataIndex, value: int32): void;

    /**
     * Get shared data value. 
     */
    //% shim=MbitMore::getMbitMoreSharedData
    function getMbitMoreSharedData(index: SharedDataIndex): int32;
}

// Auto-generated. Do not edit. Really.
