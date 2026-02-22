#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <bitset>
#include <algorithm>

using namespace std;

const double PI = 3.14159265358979323846;

// Helper to convert integer level to Binary String
string toBinary(int value, int bits) {
    string r;
    while(r.length() < bits) {
        r += (value % 2 == 0 ? "0" : "1");
        value /= 2;
    }
    reverse(r.begin(), r.end());
    return r;
}

int main(int argc, char* argv[]) {
    if (argc < 6) return 1;

    string type = argv[1];
    double freq = atof(argv[2]);
    double amp = atof(argv[3]);
    double fs = atof(argv[4]);
    int bits = atoi(argv[5]);

    int numSamples = 100; // Reduced slightly for faster web loading
    double numLevels = pow(2, bits);
    double stepSize = (2 * amp) / numLevels;

    double snr = 6.02 * bits + 1.76;

    cout << "{\"snr\": " << snr << ", \"samples\": [";

    for (int i = 0; i < numSamples; i++) {
        double t = (double)i / fs;
        double rawVal = 0;

        if (type == "sine") rawVal = amp * sin(2 * PI * freq * t);
        else if (type == "square") rawVal = (sin(2 * PI * freq * t) >= 0) ? amp : -amp;
        else if (type == "sawtooth") rawVal = 2 * amp * (t * freq - floor(0.5 + t * freq));

        // Map voltage to level index (0 to 2^N - 1)
        int levelIdx = (int)round((rawVal + amp) / stepSize);
        if (levelIdx >= numLevels) levelIdx = numLevels - 1;
        if (levelIdx < 0) levelIdx = 0;

        double quantized = (levelIdx * stepSize) - amp;
        string bin = toBinary(levelIdx, bits);

        cout << "{\"t\":" << t << ",\"orig\":" << rawVal << ",\"quant\":" << quantized << ",\"bin\":\"" << bin << "\"}";
        if (i < numSamples - 1) cout << ",";
    }

    cout << "]}";
    return 0;
}
