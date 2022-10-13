package modelApi;

import lombok.Data;

import javax.swing.*;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

@Data
public class APIKeys {
    private String geoCodingAPIKey;
    private String openWeatherAPIKey;
    private String openTripMapAPIKey;

    public APIKeys(String propertyFile) {
        Properties properties = new Properties();
        try {
            properties.load(new FileInputStream(propertyFile));
            geoCodingAPIKey = properties.getProperty("GraphHopper");
            openWeatherAPIKey = properties.getProperty("OpenWeatherMap");
            openTripMapAPIKey = properties.getProperty("OpenTripMap");

        } catch (IOException e) {
            JOptionPane.showMessageDialog(null, "ERROR: File with api keys wasn't found!");
        }
    }
}