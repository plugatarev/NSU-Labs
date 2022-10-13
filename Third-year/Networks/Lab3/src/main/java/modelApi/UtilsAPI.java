package modelApi;

import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;

import javax.swing.*;
import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.concurrent.CompletableFuture;

public class APIUtils {
    private final static int OK = 200;

    public static <T> CompletableFuture<T> sendRequest(URI uri, Class<T> tClass, String errorMessage) {
        HttpClient httpClient = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder().GET().uri(uri).build();
        return httpClient.sendAsync(request, HttpResponse.BodyHandlers.ofString()).thenApply(response -> {
             if (response.statusCode() != OK) {
                 JOptionPane.showMessageDialog(null, errorMessage);
             }
             return response;
        }).thenApply(HttpResponse::body).thenApply(s -> parseRequest(s, tClass));
    }

    private static <T> T parseRequest(String rawResp, Class<T> clazz) {
        var objectMapper = new ObjectMapper();
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);

        try {
            return objectMapper.readValue(rawResp, clazz);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(null, "ERROR: Can't parse geocoding response!");
            e.printStackTrace();
            return null;
        }
    }
}
