#include <iostream>
#include <fstream>
#include <sys/time.h>

using namespace std;

int main(int argc, char* argv[]) {

    struct timeval tv;
    unsigned long t1, t2;
    float elapse_time;
    int CHUNKS = 10240;
    
    char bytes[1024];
    for (int i = 0; i < 1024; i++) {
        bytes[i] = 0x01;
    }

    gettimeofday(&tv, NULL);
    t1 = 1000000*tv.tv_sec + tv.tv_usec;

    ofstream fout("/home/ubuntu/nfs/batchout");
    for (int i = 0; i < CHUNKS; i++) {
        fout << bytes;
    }
    fout<<"#################### END";
    fout.close();

    gettimeofday(&tv, NULL);
    t2 = 1000000*tv.tv_sec + tv.tv_usec;
    elapse_time = (float) (t2-t1) / (1000);

    cout << "Elapsed time: " << elapse_time << " milliseconds" << endl;

    return 0;
}
