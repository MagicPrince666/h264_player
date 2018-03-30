#include "cirbuffer.h"

int main()
{
    CirBuffer cirBuffer(DEFAULT_SIZE);
    const char* str = "Hello,everyone";
    char buffer[14];
    for(int i=0;i<4;i++)
    {
        cirBuffer.write(str,strlen(str));
        bzero(buffer,sizeof(buffer));
        cirBuffer.read(buffer,sizeof(buffer));
    }
    cout<<cirBuffer.getBufferSize()<<endl;
    return 0;
}
