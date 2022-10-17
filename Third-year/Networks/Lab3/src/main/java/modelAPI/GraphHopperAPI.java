package modelAPI;

import responses.GeoCoding;

import java.net.URI;
import java.util.concurrent.CompletableFuture;

public class GraphHopperAPI {
    public static CompletableFuture<GeoCoding> getAddressesByName(String name, String APIKey) {
        name = name.replace(' ', '+');
        String uriString = String.format("https://graphhopper.com/api/1/geocode?q=%s&locale=ru&key=%s", name, APIKey);
        URI uri = URI.create(uriString);
        return UtilsAPI.GETRequest(uri, GeoCoding.class,
                "ERROR: Status code of geocode response isn't 200!");
    }
}