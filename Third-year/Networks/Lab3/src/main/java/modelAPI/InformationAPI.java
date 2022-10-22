package modelAPI;

import lombok.Getter;
import responses.PlaceInformation;
import responses.WeatherDesc;

import java.util.List;
import java.util.concurrent.CompletableFuture;

public class InformationAPI {
    private static final String DEFAULT_PATH_TO_KEYS = "src/main/resources/api_keys.property";

    private static final APIKeys keys = new APIKeys(DEFAULT_PATH_TO_KEYS);

    @Getter
    public static class Information {
        List<PlaceInformation> information;
        WeatherDesc weather;

        public Information(List<PlaceInformation> information, WeatherDesc weather) {
            this.information = information;
            this.weather = weather;
        }
    }

    public static Information getInformation(double lat, double lng) {
        CompletableFuture<WeatherDesc> weather = OpenWeatherMapAPI.getWeatherByCords(lat, lng, keys.getOpenWeatherAPIKey());

        List<CompletableFuture<PlaceInformation>> places =
               OpenTripMapAPI.getInterestingPlaces(lat, lng, keys.getOpenTripMapAPIKey())
                        .thenComposeAsync(response ->
                                CompletableFuture.completedFuture(
                                        response.features()
                                                .stream()
                                                .map(k -> k.properties().xid())
                                                .map(xid -> OpenTripMapAPI.getInfoAboutPlace(xid, keys.getOpenTripMapAPIKey()))
                                                .toList())).join();
        CompletableFuture.allOf(places.toArray(new CompletableFuture[0]));
        CompletableFuture.allOf(weather);
        return new Information(places.stream().map(CompletableFuture::join).toList(), weather.join());
    }
}
