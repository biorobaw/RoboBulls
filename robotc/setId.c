string        sFileName = "id.txt";   // the name of our file

int readId(){
	TFileHandle   hFileHandle;              // will keep track of our file
  TFileIOResult nIOResult;                // will store our IO results

  int           nFileSize = 100;          // will store our file size


  int          data;                          // this will store each char as we read back in from the file
  int           nLineCounter = 0;                       // this will let us know which line we are on when reading and writing (used as the index to 'incommingString[]')

  OpenRead(hFileHandle, nIOResult, sFileName, nFileSize);
  if (nIOResult == ioRsltFileNotFound){
  	OpenWrite(hFileHandle, nIOResult, sFileName, nFileSize);
  	WriteShort(hFileHandle, nIOResult, 1);         // write 'sMessageToWrite' to the file
  	Close(hFileHandle, nIOResult);
  	OpenRead(hFileHandle, nIOResult, sFileName, nFileSize);
	}

  ReadShort(hFileHandle, nIOResult, data);
  Close(hFileHandle, nIOResult);

  return data;
}

void writeId(int id){
	TFileHandle   hFileHandle;              // will keep track of our file
  TFileIOResult nIOResult;                // will store our IO results
  int           nFileSize = 100;          // will store our file size
  Delete(sFileName,nIOResult);
	OpenWrite(hFileHandle, nIOResult, sFileName, nFileSize);
	WriteShort(hFileHandle, nIOResult, id);
	Close(hFileHandle, nIOResult);
}

task main()
{
	int id = readId();
	bool exit = false;
	nxtDisplayCenteredTextLine(1, "Current id: %d", id);
	while (!exit){
			switch (nNxtButtonPressed) {
				case kEnterButton:
					writeId(id);
					nxtDisplayCenteredTextLine(2, "Id saved", id);
					wait1Msec(300);
					break;
				case kLeftButton:
					id = id - 1;
					if (id < 0)
						id = 0;
					nxtDisplayCenteredTextLine(1, "Current id: %d", id);
					wait1Msec(300);
					break;
				case kRightButton:
					id = id + 1;
					nxtDisplayCenteredTextLine(1, "Current id: %d", id);
					wait1Msec(300);
					break;
				case kExitButton:
					exit = true;
					break;
			}
		}




}
