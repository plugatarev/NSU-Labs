package modelAPI;

import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;

import javax.swing.*;
import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.concurrent.CompletableFuture;

public class UtilsAPI {
    private final static int OK = 200;

    public static <T> CompletableFuture<T> GETRequest(URI uri, Class<T> tClass, String errorMessage) {
        HttpRequest request = HttpRequest.newBuilder().GET().uri(uri).build();
        return HttpClient.newHttpClient().sendAsync(request, HttpResponse.BodyHandlers.ofString()).thenApply(response -> {
             if (response.statusCode() != OK) {
                 JOptionPane.showMessageDialog(null, errorMessage);
             }
             return response;
        }).thenApply(HttpResponse::body).thenApply(s -> parseResponseToObject(s, tClass));
    }

    private static <T> T parseResponseToObject(String response, Class<T> tClass) {
        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
        try {
            return objectMapper.readValue(response, tClass);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(null, "ERROR: Can't parse geocoding response!");
            e.printStackTrace();
            return null;
        }
    }
}
