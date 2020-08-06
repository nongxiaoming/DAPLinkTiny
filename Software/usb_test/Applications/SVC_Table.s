

                AREA    SVC_TABLE, CODE, READONLY

                EXPORT  SVC_Count

SVC_Cnt         EQU    (SVC_End-SVC_Table)/4
SVC_Count       DCD     SVC_Cnt

; Import user SVC functions here.
                IMPORT  __SVC_1
                ;IMPORT  __SVC_2
                ;IMPORT  __SVC_3


                EXPORT  SVC_Table
SVC_Table
; Insert user SVC functions here. SVC 0 used by RTL Kernel.
                DCD     __SVC_1
                ;DCD     __SVC_2
                ;DCD     __SVC_3

SVC_End

                END

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
