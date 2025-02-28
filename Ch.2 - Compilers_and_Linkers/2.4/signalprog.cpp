#include <fftw3.h>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>

std::vector<double>* file_read(const std::string& path) {

    std::ifstream file(path);
    std::string line;

    std::vector<double>* nums = new std::vector<double>();

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        delete nums;
        return nullptr;
    }
    else {
        std::cout << "Reading File ... " << std::endl;
        while (getline(file, line)) {
            nums->push_back(std::stod(line));
        }
        std::cout << "Done!" << std::endl;
        file.close();
        return nums;
    }

}

int main () {
    std::string path = "./signal.txt";
    std::vector<double>* nums = file_read(path);

    if (nums != nullptr) {
        int n = nums->size();
        std::cout << "Performing FFT on " << n << " real data points." << std::endl;

        fftw_complex* fft_output = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (n / 2 + 1));
        fftw_plan plan_forward = fftw_plan_dft_r2c_1d(n, nums->data(), fft_output, FFTW_ESTIMATE);

        fftw_execute(plan_forward);

        std::cout << "FFT Output (Complex):" << std::endl;
        for (int i = 0; i <= n / 2; ++i) {
            std::cout << "fft_output[" << i << "] = " << fft_output[i][0] << " + " << fft_output[i][1] << "i" << std::endl;
        }

        std::vector<double> recovered_data(n);

        fftw_plan plan_backward = fftw_plan_dft_c2r_1d(n, fft_output, recovered_data.data(), FFTW_ESTIMATE);

        fftw_execute(plan_backward);

        for (int i = 0; i < n; ++i) {
            recovered_data[i] /= n;
        }

        std::cout << "\nRecovered Real Data:" << std::endl;
        for (int i = 0; i < std::min(n, 10); ++i) {  // Print the first 10 elements
            std::cout << "recovered_data[" << i << "] = " << recovered_data[i] << std::endl;
        }

        // Clean up FFTW resources
        fftw_destroy_plan(plan_forward);
        fftw_destroy_plan(plan_backward);
        fftw_free(fft_output);
        delete nums;

        return 0;
    }
    else {
        return 1;
    }

    
}