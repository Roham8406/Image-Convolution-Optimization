long long threshold(unsigned char* out, unsigned char threshold, long long size) {
    long long sum = 0;
    while (size--) {
        if (threshold <= out[size]) sum++;
    }
    return sum;
}