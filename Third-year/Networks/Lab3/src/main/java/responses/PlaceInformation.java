package utils;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import lombok.Getter;

@Getter
//@JsonIgnoreProperties(ignoreUnknown = false)
public class OTMPlaceInfo {
    private String xid;
    private String name;
    private Address address;
    private OtmInfo info;

    public static class OtmInfo {
        @Getter
        String description;
    }

    @Override
    public String toString() {
        String desc = name + "\n" + address.toString() + "\n";
        if (info != null) {
            desc += formattedDescr(info.getDescription());
        } else {
            desc += "No description was found for this place\n";
        }
        return desc;
    }
    private String formattedDescr(String descr) {
        if (descr == null) {
            return "No description was found for this place\n";
        }
        StringBuilder stringBuilder = new StringBuilder();
        for(int i = 0; i < descr.length(); i++) {
            stringBuilder.append(descr.charAt(i));
            if (descr.charAt(i) == '.') {
                stringBuilder.append('\n');
            }
        }
        return stringBuilder.toString();
    }
}