# windows-remote-ime
Using Android mobile as wireless handwriting board ( PC side )
This's based on Microsoft Sample IME.


### Install :
    1.Copy SampleIME.dll & SampleIMESimplifiedQuanPin.txt to C:\Program Files (x86)\SampleIME
    2.Type following command at command prompt (administrator)
        regsvr32 "C:\Program Files (x86)\SampleIME\SampleIME.dll"
  
### Unistall :
    regsvr32 /u "C:\Program Files (x86)\SampleIME\SampleIME.dll"
