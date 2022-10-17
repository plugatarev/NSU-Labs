package responses;

import com.fasterxml.jackson.annotation.JsonAlias;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import lombok.Getter;

@Getter
public class Weather {
    @JsonAlias("main")
    private Main mainInformation;

    @JsonAlias("weather")
    private Description description;

    @JsonAlias("wind")
    private Wind wind;

    @Getter
    public static class Main {
        private double temp;
        private double feels_like;
        private double humidity;
    }

    @Getter
    public static class Wind {
        private double speed;
    }
    @Getter
    public static class Description {
        private String main;
        private String description;
    }
}