package responses;

import com.fasterxml.jackson.annotation.JsonAlias;
import lombok.Getter;

import java.util.ArrayList;

@Getter
public class WeatherDesc {
    @JsonAlias("main")
    private Main mainInformation;

    @JsonAlias("weather")
    private ArrayList<Weather> weather;

    @JsonAlias("wind")
    private Wind wind;

    public record Main(double temp, double feels_like, double humidity) {}

    public record Wind(double speed) {}

    public record Weather(String main, String description) {}
}