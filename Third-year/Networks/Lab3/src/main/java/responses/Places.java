package responses;

import com.fasterxml.jackson.annotation.JsonAlias;

import java.util.ArrayList;

public record Places(@JsonAlias("features") ArrayList<Feature> features) {
    public record Feature(@JsonAlias("properties") Properties properties) {}
    public record Properties(String xid, String name, String kinds) {}
}