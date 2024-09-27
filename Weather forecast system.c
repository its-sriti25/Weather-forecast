#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ANSI color codes
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Structure to store weather data
typedef struct {
    char weather[20];
    float temperature;
    int humidity;
    float windSpeed;
    char conditions[50];
    char date[11];  // Format: YYYY-MM-DD
    char location[50];
} WeatherData;

// Structure to store forecast information
typedef struct {
    char date[11];  // Format: YYYY-MM-DD
    char day[10];   // Day of the week
    WeatherData data;
} WeatherForecast;

// Function to get current date in YYYY-MM-DD format
void getCurrentDate(char *buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// Function to get the day of the week
void getDayOfWeek(const char *date, char *buffer) {
    struct tm tm = {0};
    sscanf(date, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
    tm.tm_year -= 1900; // Year since 1900
    tm.tm_mon -= 1;     // Months are zero-indexed in tm structure
    time_t t = mktime(&tm);
    strftime(buffer, 10, "%A", localtime(&t));
}


// Function to generate random weather data
void generateRandomWeatherData(WeatherData *weatherData) {
    const char *weatherTypes[] = {"Sunny", "Cloudy", "Rainy", "Stormy", "Snowy"};
    const char *conditions[] = {"Clear sky", "Partly cloudy", "Overcast", "Light rain", "Heavy rain"};
    int weatherIndex = rand() % 5;
    int conditionIndex = rand() % 5;
    strcpy(weatherData->weather, weatherTypes[weatherIndex]);
    weatherData->temperature = (float)(rand() % 35) + 5.0;
    weatherData->humidity = rand() % 100 + 1;
    weatherData->windSpeed = (float)(rand() % 50) + 1.0;
    strcpy(weatherData->conditions, conditions[conditionIndex]);
}

// Function to generate forecast data for the next 5 days
void generateForecasts(WeatherData *currentWeather, WeatherForecast forecasts[5]) {
    for (int i = 0; i < 5; ++i) {
        // Calculate the date for the forecast
        time_t t = time(NULL) + (i + 1) * 24 * 60 * 60; // Increment by i+1 days
        struct tm tm = *localtime(&t);
        sprintf(forecasts[i].date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

        // Get the day of the week
        getDayOfWeek(forecasts[i].date, forecasts[i].day);

        // Generate random weather data for the forecast
        generateRandomWeatherData(&forecasts[i].data);
        strcpy(forecasts[i].data.date, forecasts[i].date);
        strcpy(forecasts[i].data.location, currentWeather->location);
    }
}

// Function to print a line separator
void printLineSeparator() {
    printf(BLUE "--------------------------------------------------" RESET "\n");
}

// Function to print the weather data
void printWeatherData(WeatherData *weatherData) {
    printf(YELLOW "Location    :" RESET " %s\n", weatherData->location);
    printf(YELLOW "Date        :" RESET " %s\n", weatherData->date);
    printf(YELLOW "Weather     :" RESET " %s\n", weatherData->weather);
    printf(YELLOW "Temperature :" RESET " %.2f°C\n", weatherData->temperature);
    printf(YELLOW "Humidity    :" RESET " %d%%\n", weatherData->humidity);
    printf(YELLOW "Wind Speed  :" RESET " %.2f km/h\n", weatherData->windSpeed);
    printf(YELLOW "Conditions  :" RESET " %s\n", weatherData->conditions);
}

// Function to print the forecast
void printForecasts(WeatherForecast forecasts[5]) {
    printf(CYAN "\n\n5-Day Weather Forecast:\n" RESET);
    printLineSeparator();
    for (int i = 0; i < 5; ++i) {
        printf(GREEN "Date: %s (%s)\n" RESET, forecasts[i].date, forecasts[i].day);
        printWeatherData(&forecasts[i].data);
        printLineSeparator();
    }
}

int main() {
    srand(time(0)); // Seed the random number generator

    WeatherData currentWeather;
    WeatherForecast forecasts[5];

    // Get user input for location
    printf("Enter your location: ");
    fgets(currentWeather.location, sizeof(currentWeather.location), stdin);
    currentWeather.location[strcspn(currentWeather.location, "\n")] = '\0'; // Remove the newline character

    // Get current date
    getCurrentDate(currentWeather.date);

    // Generate current weather data
    generateRandomWeatherData(&currentWeather);

    // Generate 5-day forecasts
    generateForecasts(&currentWeather, forecasts);

    // Print current weather data
    printf(BOLD "\nCurrent Weather:\n" RESET);
    printLineSeparator();
    printWeatherData(&currentWeather);
    printLineSeparator();

    // Print 5-day forecasts
    printForecasts(forecasts);

    return 0;
}