// tests go here; this will not be compiled when this package is used as a library
CodeMore.onDisplayTextCommand(function (cmd) {
    コマンド配列 = cmd.split(" ")
    コマンド部 = コマンド配列[0]
    コマンド部 = コマンド部.toLowerCase()
if (コマンド部 == "@cmd") {
        第1引数 = コマンド配列[1]
        if (第1引数 == "1") {
            basic.showIcon(IconNames.Yes)
        } else {
            basic.showIcon(IconNames.Diamond)
        }
    } else {
        basic.showString("Send \"@cmd arg1\"")
    }
})
let 第1引数 = ""
let コマンド配列: string[] = []
let コマンド部 = ""
CodeMore.startService()
