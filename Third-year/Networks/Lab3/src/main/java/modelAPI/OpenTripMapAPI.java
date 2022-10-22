package modelAPI;

import responses.PlaceInformation;
import responses.Places;

import java.net.URI;
import java.util.concurrent.CompletableFuture;

public class OpenTripMapAPI {
    public static CompletableFuture<Places> getInterestingPlaces(double lat, double lon, String APIKey) {
        String latS = Double.toString(lat);
        String lonS = Double.toString(lon);
        String uriString = String.format(
                "https://api.opentripmap.com/0.1/ru/places/radius?radius=5000&rate=3&lon=%s&lat=%s&apikey=%s",
                lonS,
                latS,
                APIKey
        );
        URI uri = URI.create(uriString);
        return UtilsAPI.GETRequest(uri, Places.class,
                "ERROR: Status code of Open Trip map (places) response is ");
    }

    public static CompletableFuture<PlaceInformation> getInfoAboutPlace(String xid, String APIKey) {
        String uriString = String.format("https://api.opentripmap.com/0.1/ru/places/xid/%s?apikey=%s", xid, APIKey);
        URI uri = URI.create(uriString);
        return UtilsAPI.GETRequest(uri, PlaceInformation.class,
                "ERROR: Status code of Open Trip map (info) response is ");
    }
}