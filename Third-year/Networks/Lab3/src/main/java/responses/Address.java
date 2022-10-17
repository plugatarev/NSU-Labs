package utils;

public record Address(String osm_id, String osm_type,
                      String country, String osm_key,
                      String city, String osm_value,
                      String postcode, String name,
                      Point point) {

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        if (country != null) stringBuilder.append(country).append(",");
        if (city != null) stringBuilder.append(city).append(",");
        if (name != null) stringBuilder.append(name).append(",");
        if (postcode != null) stringBuilder.append(postcode);
        if (stringBuilder.charAt(stringBuilder.length() - 1) == ',') stringBuilder.deleteCharAt(stringBuilder.length() - 1);
        return stringBuilder.toString();
    }
}