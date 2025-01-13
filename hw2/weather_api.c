#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

struct memory {
    char *response;
    size_t size;
};

static size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realsize = size * nmemb;
    struct memory *mem = clientp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(!ptr) {
        return 0;  /* out of memory */
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

static char * weatharCodes[][255] = {
    [113] = "Sunny",
    [116] = "PartlyCloudy",
    [119] = "Cloudy",
    [122] = "VeryCloudy",
    [143] = "Fog",
    [176] = "LightShowers",
    [179] = "LightSleetShowers",
    [182] = "LightSleet",
    [185] = "LightSleet",
    [200] = "ThunderyShowers",
    [227] = "LightSnow",
    [230] = "HeavySnow",
    [248] = "Fog",
    [260] = "Fog",
    [263] = "LightShowers",
    [266] = "LightRain",
    [281] = "LightSleet",
    [284] = "LightSleet",
    [293] = "LightRain",
    [296] = "LightRain",
    [299] = "HeavyShowers",
    [302] = "HeavyRain",
    [305] = "HeavyShowers",
    [308] = "HeavyRain",
    [311] = "LightSleet",
    [314] = "LightSleet",
    [317] = "LightSleet",
    [320] = "LightSnow",
    [323] = "LightSnowShowers",
    [326] = "LightSnowShowers",
    [329] = "HeavySnow",
    [332] = "HeavySnow",
    [335] = "HeavySnowShowers",
    [338] = "HeavySnow",
    [350] = "LightSleet",
    [353] = "LightShowers",
    [356] = "HeavyShowers",
    [359] = "HeavyRain",
    [362] = "LightSleetShowers",
    [365] = "LightSleetShowers",
    [368] = "LightSnowShowers",
    [371] = "HeavySnowShowers",
    [374] = "LightSleetShowers",
    [377] = "LightSleet",
    [386] = "ThunderyShowers",
    [389] = "ThunderyHeavyRain",
    [392] = "ThunderySnowShowers",
    [395] = "HeavySnowShowers",
};

char * getWindDirection(int degrees);

int main(const int argc, const char * argv[])
{
    if (argc != 2) {
        printf("Usage: %s town_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    CURL *curl = curl_easy_init();
    if(!curl) {
        exit(EXIT_FAILURE);
    }
    struct memory chunk = {NULL};
    json_object *currentCities;
    json_object *tempC;
    json_object *weatherCode;
    json_object *winddirDegree;
    json_object *windspeedKmph;
    json_object *nearest_areas;
    json_object *nearest_area_cities;
    json_object *nearest_area_city_name;
    json_object *nearest_area_countries;
    json_object *nearest_area_country_name;
    char url[255] = "";
    sprintf(url, "https://wttr.in/%s?format=j1", argv[1]);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    CURLcode res = curl_easy_perform(curl);
    if(CURLE_OK != res) {
        exit(EXIT_FAILURE);
    }
    const json_object *response = json_tokener_parse(chunk.response);
    json_object_object_get_ex(response, "current_condition", &currentCities);
    const json_object *currentCity = json_object_array_get_idx(currentCities, 0);
    json_object_object_get_ex(currentCity, "temp_C", &tempC);
    json_object_object_get_ex(currentCity, "weatherCode", &weatherCode);
    json_object_object_get_ex(currentCity, "winddirDegree", &winddirDegree);
    json_object_object_get_ex(currentCity, "windspeedKmph", &windspeedKmph);
    json_object_object_get_ex(response, "nearest_area", &nearest_areas);
    const json_object *nearest_area = json_object_array_get_idx(nearest_areas, 0);

    json_object_object_get_ex(nearest_area, "areaName", &nearest_area_cities);
    const json_object *nearest_area_city = json_object_array_get_idx(nearest_area_cities, 0);
    json_object_object_get_ex(nearest_area_city, "value", &nearest_area_city_name);

    json_object_object_get_ex(nearest_area, "country", &nearest_area_countries);
    const json_object *nearest_area_country = json_object_array_get_idx(nearest_area_countries, 0);
    json_object_object_get_ex(nearest_area_country, "value", &nearest_area_country_name);

    printf(
        "Weather forecast for %s %s: \nTemperature is %d °C\nWind: %s %d km/h\n%s\n",
        json_object_get_string(nearest_area_city_name),
        json_object_get_string(nearest_area_country_name),
        json_object_get_int(tempC),
        getWindDirection(json_object_get_int(winddirDegree)),
        json_object_get_int(windspeedKmph),
        *weatharCodes[json_object_get_int(weatherCode)]
    );
    free(chunk.response);
    curl_easy_cleanup(curl);

    return EXIT_SUCCESS;
}

char* getWindDirection(const int degrees)
{
    if(degrees == 0 || degrees == 360) {
        return "North";
    } else if(degrees < 90) {
        return "North west";
    } else if(degrees == 90) {
        return "West";
    } else if(degrees > 90 && degrees < 180) {
        return "South west";
    } else if(degrees == 180) {
        return "South";
    } else if(degrees > 180 && degrees < 270) {
        return "South east";
    } else if(degrees == 270) {
        return "East";
    } else if(degrees > 270 && degrees < 360) {
        return "North east";
    }

    return "Unknown";
}