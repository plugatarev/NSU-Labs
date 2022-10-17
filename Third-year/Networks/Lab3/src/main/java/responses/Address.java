package responses;

public record Address(String country, String city, String postcode, String name, Point point) {
    public record Point(double lat, double lng) {}

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