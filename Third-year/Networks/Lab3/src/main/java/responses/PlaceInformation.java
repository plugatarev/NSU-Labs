package responses;

import com.fasterxml.jackson.annotation.JsonAlias;

public record PlaceInformation(String xid, String name, Address address, @JsonAlias("wikipedia_extracts") Information info) {

    public record Information(String text) {}

    @Override
    public String toString() {
        String desc = name + "\n" + address.toString() + "\n";

        if (info != null) {
            desc += formattedDescription(info.text());
        }
        else {
            desc += "No description was found for this place\n";
        }

        return desc;
    }

    private String formattedDescription(String description) {
        if (description == null) {
            return "No description was found for this place\n";
        }
        StringBuilder stringBuilder = new StringBuilder();
        int length = description.length();
        int count = 0;
        for (int i = 0; i < length; i++) {
            if (count++ == 100) {
                count = 0;
                stringBuilder.append("\n");
            }
            stringBuilder.append(description.charAt(i));
        }
        return stringBuilder.toString();
    }
}