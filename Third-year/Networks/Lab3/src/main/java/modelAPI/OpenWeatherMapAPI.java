package modelAPI;

import responses.WeatherDesc;

import java.net.URI;
import java.util.concurrent.CompletableFuture;

public class OpenWeatherMapAPI {
    public static CompletableFuture<WeatherDesc> getWeatherByCords(double lat, double lng, String APIKey) {
        String uriString = String.format(
                "https://api.openweathermap.org/data/2.5/weather?lat=%f&lon=%f&appid=%s",lat, lng, APIKey);
        URI uri = URI.create(uriString);
        return UtilsAPI.GETRequest(uri, WeatherDesc.class,
                "ERROR: Status code of Open Weather Map response is ");
    }
}