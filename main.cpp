#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random> // ✅ Added for mt19937

using namespace std;
using namespace sf;

const int WIDTH = 1200;
const int HEIGHT = 700;
const int BAR_WIDTH = 5;
const size_t NUM_BARS = WIDTH / BAR_WIDTH;
const int DELAY_MS = 5; // Animation speed

// Function to draw bars
void drawBars(RenderWindow &window, const vector<int> &arr, int highlight1 = -1, int highlight2 = -1) {
    window.clear(Color::Black);
    for (size_t i = 0; i < arr.size(); ++i) {
        RectangleShape bar(Vector2f(BAR_WIDTH - 1, arr[i]));
        bar.setPosition(static_cast<float>(i * BAR_WIDTH), HEIGHT - arr[i]);

        if (static_cast<int>(i) == highlight1 || static_cast<int>(i) == highlight2)
            bar.setFillColor(Color::Red);
        else
            bar.setFillColor(Color::White);

        window.draw(bar);
    }
    window.display();
}

// Handle window close events
bool handleWindowEvents(RenderWindow &window) {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed)
            window.close();
    }
    return window.isOpen();
}

// Bubble Sort
void bubbleSort(RenderWindow &window, vector<int> &arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        for (size_t j = 0; j < arr.size() - i - 1; ++j) {
            if (!handleWindowEvents(window)) return;
            drawBars(window, arr, static_cast<int>(j), static_cast<int>(j + 1));
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }
}

// Insertion Sort
void insertionSort(RenderWindow &window, vector<int> &arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && arr[j] > key) {
            if (!handleWindowEvents(window)) return;
            arr[j + 1] = arr[j];
            drawBars(window, arr, j, j + 1);
            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
            j--;
        }
        arr[j + 1] = key;
    }
}

// Selection Sort
void selectionSort(RenderWindow &window, vector<int> &arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (!handleWindowEvents(window)) return;
            drawBars(window, arr, static_cast<int>(minIndex), static_cast<int>(j));
            if (arr[j] < arr[minIndex]) minIndex = j;
            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
        swap(arr[i], arr[minIndex]);
    }
}

// Merge Sort Helpers
void merge(RenderWindow &window, vector<int> &arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);
    size_t i = 0, j = 0;
    int k = l;

    while (i < left.size() && j < right.size()) {
        if (!handleWindowEvents(window)) return;
        drawBars(window, arr, k);
        if (left[i] <= right[j]) arr[k++] = left[i++];
        else arr[k++] = right[j++];
        this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
    }

    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

void mergeSort(RenderWindow &window, vector<int> &arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(window, arr, l, m);
        mergeSort(window, arr, m + 1, r);
        merge(window, arr, l, m, r);
    }
}

// Quick Sort Helpers
int partition(RenderWindow &window, vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (!handleWindowEvents(window)) return -1;
        drawBars(window, arr, j, high);
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
        this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(RenderWindow &window, vector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = partition(window, arr, low, high);
        if (pi == -1) return;
        quickSort(window, arr, low, pi - 1);
        quickSort(window, arr, pi + 1, high);
    }
}

// Heap Sort Helpers
void heapify(RenderWindow &window, vector<int> &arr, size_t n, size_t i) {
    size_t largest = i;
    size_t l = 2 * i + 1;
    size_t r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        drawBars(window, arr, static_cast<int>(i), static_cast<int>(largest));
        this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        heapify(window, arr, n, largest);
    }
}

void heapSort(RenderWindow &window, vector<int> &arr) {
    size_t n = arr.size();
    for (int i = static_cast<int>(n / 2) - 1; i >= 0; i--)
        heapify(window, arr, n, i);
    for (int i = static_cast<int>(n) - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        drawBars(window, arr, 0, i);
        this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        heapify(window, arr, i, 0);
    }
}

// Main
int main() {
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Sorting Algorithm Visualizer - C++ SFML");
    srand(static_cast<unsigned>(time(0)));

    vector<int> arr(NUM_BARS);
    for (int &a : arr) a = rand() % HEIGHT;

    // Random shuffle using modern C++ RNG
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::shuffle(arr.begin(), arr.end(), rng);

    cout << "Choose Sorting Algorithm:\n";
    cout << "1. Bubble Sort\n2. Insertion Sort\n3. Selection Sort\n4. Merge Sort\n5. Quick Sort\n6. Heap Sort\n";
    int choice;
    cin >> choice;

    drawBars(window, arr);

    switch (choice) {
        case 1: bubbleSort(window, arr); break;
        case 2: insertionSort(window, arr); break;
        case 3: selectionSort(window, arr); break;
        case 4: mergeSort(window, arr, 0, static_cast<int>(arr.size()) - 1); break;
        case 5: quickSort(window, arr, 0, static_cast<int>(arr.size()) - 1); break;
        case 6: heapSort(window, arr); break;
        default: cout << "Invalid choice!\n"; break;
    }

    drawBars(window, arr);
    while (window.isOpen()) {
        if (!handleWindowEvents(window)) break;
    }

    return 0;
}