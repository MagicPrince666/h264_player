#ifndef __CIRBUFFER__H
#define __CIRBUFFER__H

#include <iostream>
#include <stdlib.h>
#include <string.h>
//#include <boost/assert.hpp>

using namespace std;
using namespace boost;

#define DEFAULT_SIZE 20
class CirBuffer
{
    public:
        CirBuffer(int maxSize):bufferSize(maxSize)
        {
            readPtr = writePtr = 0;
            isReadFast = isWriteFast = false;
            buffer = new char[bufferSize];
            assert(buffer);
            bzero(buffer,bufferSize);
        }
        ~CirBuffer()
        {
            assert(buffer);
            delete[] buffer;
        }

        int getBufferSize()
        {
            return bufferSize;
        }

        void display()
        {
            cout<<"writePtr:"<<writePtr<<"  readPtr:"<<readPtr<<" buffer:"<<buffer<<endl;
        }

        int usedMemorySize()
        {
            int len = 0;
            if(writePtr > readPtr)
                len = writePtr - readPtr;
            else if(readPtr > writePtr)
                len = bufferSize - readPtr + writePtr;
            return len;
        }
        void addMemory(int size,int len)
        {
            assert(buffer);
            int freelen = bufferSize - len;
            while(freelen < size)
            {
                bufferSize = bufferSize<<1;
                freelen = bufferSize - len;
            }

            char* newBuffer = new char[bufferSize];
            assert(newBuffer);
            bzero(newBuffer,bufferSize);
            if(writePtr > readPtr)
            {
                memcpy(newBuffer,&buffer[readPtr],writePtr-readPtr);
                delete[] buffer;
                buffer = newBuffer;
                readPtr = 0;
                writePtr = writePtr-readPtr;
            }
            else
            {
                int end = bufferSize - readPtr;
                memcpy(newBuffer,&buffer[readPtr],end);
                memcpy(&newBuffer[end],buffer,writePtr);
                delete[] buffer;
                buffer = newBuffer;
                readPtr = 0;
                writePtr = end + writePtr;
            }
        }

        void reserveBuffer(int size)
        {
            int usedLen = usedMemorySize();
            if(bufferSize < usedLen + size)
                addMemory(size,usedLen);
        }
        void readMv(int len)
        {
            if(len > bufferSize) return;
            readPtr = (readPtr + len) %(bufferSize);
            if(readPtr == writePtr)
                isReadFast = true;
            else
                isReadFast = false;
            display();
        }
        void writeMv(int len)
        {
            if(len > bufferSize) return;
            writePtr = (writePtr + len) %(bufferSize);
            if(writePtr == readPtr)
                isWriteFast = true;
            else
                isWriteFast = false;
            display();
        }

        void write(const char* Buffer,int size)
        {
            assert(Buffer);
            reserveBuffer(size);
            if(writePtr > readPtr)
            {
                int end = bufferSize - writePtr;
                if(end >= size)
                {
                    memcpy(&buffer[writePtr],Buffer,size);
                    writeMv(size);
                }
                else
                {
                    memcpy(&buffer[writePtr],Buffer,end);
                    memcpy(buffer,&Buffer[end],size-end);
                    writeMv(size);
                }
            }
            else if(writePtr < readPtr)
            {
                int len = readPtr - writePtr;
                if(len >= size)
                {
                    memcpy(&buffer[writePtr],Buffer,size);
                    writeMv(size);
                }
            }
            else
            {
                if(isReadFast)
                {
                    int end = bufferSize - writePtr;
                    if(end >= size)
                    {
                        memcpy(&buffer[writePtr],Buffer,size);
                        writeMv(size);
                    }
                    else
                    {
                        memcpy(&buffer[writePtr],Buffer,end);
                        memcpy(buffer,&Buffer[end],size-end);
                        writeMv(size);
                    }
                }
                else if(!isWriteFast && !isReadFast)
                {
                    memcpy(&buffer[writePtr],Buffer,size);
                    writeMv(size);
                }
            }
        }

        void read(char* Buffer,int size)
        {
            assert(Buffer);
            if(writePtr > readPtr)
            {
                int len = writePtr - readPtr;
                int readlen = (len > size)?size:len;
                memcpy(Buffer,&buffer[readPtr],readlen);
                readMv(readlen);
            }
            else if(writePtr < readPtr)
            {
                int end = bufferSize - readPtr;
                if(end >= size)
                {
                    memcpy(Buffer,&buffer[readPtr],size);
                    readMv(size);
                }
                else
                {
                    int len = (size > end + readPtr)?(readPtr):(size-end);
                    memcpy(Buffer,&buffer[readPtr],end);
                    memcpy(&Buffer[end],buffer,len);
                    readMv(len+end);
                }
            }
            else
            {
                if(isWriteFast)
                {
                    int end = bufferSize - readPtr;
                    if(end >= size)
                    {
                        memcpy(Buffer,&buffer[readPtr],size);
                        readMv(size);
                    }
                    else
                    {
                        memcpy(Buffer,&buffer[readPtr],end);
                        memcpy(&Buffer[end],buffer,size-end);
                        readMv(size);
                    }
                }
            }
        }
    private:
        char* buffer;
        int readPtr;
        int writePtr;
        int bufferSize;
        bool isReadFast;
        bool isWriteFast;
};
#endif
