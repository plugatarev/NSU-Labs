package responses;

import java.util.ArrayList;

public record GeoCoding(ArrayList<Address> hits) {}