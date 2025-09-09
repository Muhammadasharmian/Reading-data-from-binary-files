#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>

//Error checking code

ssize_t checkError(ssize_t val, const char* msg)
{
    if (val == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

//Functions for easier conversion 

double accCalc(int16_t v) { return (double)v / 32768.0 * 16.0;}
double gyroCalc(int16_t v) { return (double)v / 32768.0 * 2000.0;}
double angleCalc(int16_t v) { return (double)v / 32768.0 * 180.0;}

// Main

int main(int argc, char* argv[])
{
    ssize_t numRd;
    int fd  ;
    uint8_t buffer[20];
    double values[9]; // 9 doubles per packet

    // Open the file
    fd = checkError(open(argv[1], O_RDONLY),"Failed to open file");

    // Processing each packet 
    while ((numRd = read(fd, buffer, 20)) > 0){
        int16_t ax = (buffer[3] << 8) | buffer[2];
        int16_t ay = (buffer[5] << 8) | buffer[4];
        int16_t az = (buffer[7] << 8) | buffer[6];

        int16_t wx = (buffer[9] << 8) | buffer[8];
        int16_t wy = (buffer[11] << 8) | buffer[10];
        int16_t wz = (buffer[13] << 8) | buffer[12];

        int16_t roll = (buffer[15] << 8) | buffer[14];
        int16_t pitch = (buffer[17] << 8) | buffer[16];
        int16_t yaw = (buffer[19] << 8) | buffer[18];

        // Calculating the values and storing them 
        values[0] = accCalc(ax);
        values[1] = accCalc(ay);
        values[2] = accCalc(az);

        values[3] = gyroCalc(wx);
        values[4] = gyroCalc(wy);
        values[5] = gyroCalc(wz);

        values[6] = angleCalc(roll);
        values[7] = angleCalc(pitch);
        values[8] = angleCalc(yaw);

        // Printing the values 
        for (int i = 0; i < 9; i++) {
            printf("%10.6f ", values[i]);
        }
        printf("\n");
    }

    close(fd);

    return 0;
}
