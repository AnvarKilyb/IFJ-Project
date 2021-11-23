
//const char start[] = ".IFJcode21";
//
//void start_generet(/*tree*/){
//
//}

/**/
/* FJcode21
 *
 * Function start:
 *      PUSHFRAME
 *
 *
 *  While:
 *      ls: -- loop start
 *      LT bool LF@a LF@b -- <  |  JUMPIFEQ lb LF@a LF@b //  == |  JUMPIFNEQ lb LF@a LF@b //  != | GT bool LF@a LF@b -- >
 *      JUMPIFEQ lb bool true   |                               |                                | JUMPIFEQ lb bool true
 *      JUMP lend
 *      lb:
 *
 *      JUMP ls:
 *      lend:
 *
 *  IF:
 *      LT bool LF@a LF@b -- <   |  JUMPIFEQ ift LF@a LF@b //  == |  JUMPIFNEQ ift LF@a LF@b //  != | GT bool LF@a LF@b -- >
 *      JUMPIFEQ ift bool true   |                                |                                 | JUMPIFEQ ift bool true
 *
 *
 *      JUMP ifend
 *      ift:
 *
 *      ifend:
 *
 *
 * ///// function
 *  reads -> LF@_string
 *           READ LF@_string string
 *
 *  write -> WRITE LF@w
 *
 *  substr -> POP LF@str
 *            POP LF@1p
 *            POP LF@2p
 *            DEFVAR LF@char
 *            DEFVAR LF@count_1
 *            MOV LF@count_1 LF@1p
 *            DEFVAR LF@count_2
 *            MOV LF@count_2 0
 *
 *            ls:
 *            LT bool LF@count_1 LF@2p
 *            JUMPIFEQ lb bool true
 *            JUMPIFEQ lb LF@count_1 LF@2p
 *            JUMP lend:
 *            lb:
 *            GETCHAR LF@char LF@str LF@count_1
 *            SETCHAR LF@char LF@count_2 LF@char
 *
 *            ADD LF@count_1 1
 *            ADD LF@count_2 1
 *            JUMP ls
 *            lend:
 *
 */
