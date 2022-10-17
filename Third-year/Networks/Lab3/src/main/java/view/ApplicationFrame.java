package view;

import modelAPI.APIKeys;
import modelAPI.GraphHopperAPI;
import modelAPI.OpenTripMapAPI;
import modelAPI.OpenWeatherMapAPI;
import responses.*;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

public class ApplicationFrame {
    private static final int FRAME_WIDTH = 800;
    private static final int FRAME_HEIGHT = 800;
    private static final String DEFAULT_PATH_TO_KEYS = "src/main/resources/api_keys.property";

    private final APIKeys keys = new APIKeys(DEFAULT_PATH_TO_KEYS);
    private final ArrayList<Address.Point> geoPoints = new ArrayList<>();
    private final ArrayList<String> xids = new ArrayList<>();

    private final JFrame frame = new JFrame("GeoFinder");
    private final JTextField placeName = new JTextField();

    private final JButton findButton = new JButton("Find place");
    private final JButton infoAboutPlaceButton = new JButton("Get information");
    private final JButton moreInfoAboutPlaceButton = new JButton("More information about place");

    private final DefaultListModel<String> addressesListModel = new DefaultListModel<>();
    private final JList<String> addressesList = new JList<>(addressesListModel);

    private final JLabel weatherLabel = new JLabel();

    private final DefaultListModel<String> interestingPlacesModel = new DefaultListModel<>();
    private final JList<String> interestingPlacesList = new JList<>(interestingPlacesModel);

    private final JScrollPane addressesScrollPane = new JScrollPane(addressesList);
    private final JScrollPane interestingPlacesScrollPane = new JScrollPane(interestingPlacesList);

    public ApplicationFrame() {
        frame.setSize(FRAME_WIDTH, FRAME_HEIGHT);
        frame.setResizable(false);
        frame.setLayout(new VerticalLayout());
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        addressesList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        interestingPlacesList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        setSizes();

        addComponents();
        addActionListeners();
    }
    public void start() {
        frame.setVisible(true);
    }

    private void updateGeoCoding(GeoCoding geo) {
        if (geo == null) return;
        addressesListModel.clear();
        geoPoints.clear();
        ArrayList<Address> addresses = geo.hits();
        int size = addresses.size();
        for (int i = 0; i < size; i++) {
            addressesListModel.add(i, addresses.get(i).toString());
            geoPoints.add(i, addresses.get(i).point());
        }
    }

    private void updateWeatherInfo(WeatherDesc weatherDesc) {
        if (weatherDesc == null) return;
        weatherLabel.setText(
                "<html> Weather: " + weatherDesc.getWeather().get(0).description() + ", " + weatherDesc.getWeather().get(0).main() +
                "<br> Temperature: " + ((int)weatherDesc.getMainInformation().temp() - 273) + "°C" +
                "<br> Temperature feels like: " + ((int)weatherDesc.getMainInformation().feels_like() - 273) + "°C" +
                "<br> Wind speed: " + weatherDesc.getWind().speed() +
                "<br> Humidity: " + weatherDesc.getMainInformation().humidity() +
                "</html>");
    }

    private Places updateInterestingPlaces(Places resp) {
        interestingPlacesModel.clear();
        if (resp == null) return null;
        ArrayList<Places.Feature> places = resp.features();
        if (places.size() == 0) {
            interestingPlacesModel.add(0, "No interesting places was found");
        }
        for (int i = 0; i < places.size() && i < 10; i++) {
            interestingPlacesModel.add(i, (i + 1) + ". " + places.get(i).properties().name());
        }
        return resp;
    }

    private void updateXidsPlaces(Places places) {
        xids.clear();
        if (places == null) return;
        ArrayList<Places.Feature> placesFeatures = places.features();
        for (int i = 0; i < placesFeatures.size(); i++) {
            String xid = placesFeatures.get(i).properties().xid();
            xids.add(i, xid);
        }
    }

    private void showInfoAboutInterestingPlace(PlaceInformation info) {
        JTextArea textArea = new JTextArea(30, 70);
        textArea.setFont(new Font(Font.DIALOG, Font.BOLD,15));
        textArea.setText(info.toString());
        textArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(textArea);
        JOptionPane.showMessageDialog(frame, scrollPane);
    }

    private void addComponents() {
        addressesListModel.add(0, "Please, type name of place you want to know about and click \"Find!\"");
        frame.add(placeName);
        frame.add(findButton);
        frame.add(addressesScrollPane);
        frame.add(infoAboutPlaceButton);
        frame.add(weatherLabel);
        frame.add(interestingPlacesScrollPane);
        frame.add(moreInfoAboutPlaceButton);
    }

    private void setSizes() {
        addressesList.setPreferredSize(new Dimension(500, 200));
        interestingPlacesList.setPreferredSize(new Dimension(500, 200));
        placeName.setPreferredSize(new Dimension(200, 20));
        weatherLabel.setPreferredSize(new Dimension(400, 100));
    }

    private void addActionListeners() {
        findButton.addActionListener((l) -> {
            String placeName = this.placeName.getText();
            if (placeName != null) {
                GraphHopperAPI.getAddressesByName(placeName, keys.getGeoCodingAPIKey()).
                              thenAccept(this::updateGeoCoding);
            }
        });

        infoAboutPlaceButton.addActionListener(l -> {
            double lat = geoPoints.get(addressesList.getSelectedIndex()).lat();
            double lng = geoPoints.get(addressesList.getSelectedIndex()).lng();
            OpenWeatherMapAPI.getWeatherByCords(lat, lng, keys.getOpenWeatherAPIKey())
                             .thenAccept(this::updateWeatherInfo);
            OpenTripMapAPI.getInterestingPlaces(lat, lng, keys.getOpenTripMapAPIKey())
                          .thenApply(this::updateInterestingPlaces)
                          .thenAccept(this::updateXidsPlaces);
        });

        moreInfoAboutPlaceButton.addActionListener(l -> {
            int index = interestingPlacesList.getSelectedIndex();
            String xid = xids.get(index);
            OpenTripMapAPI.getInfoAboutPlace(xid, keys.getOpenTripMapAPIKey())
                          .thenAccept(this::showInfoAboutInterestingPlace);
        });
    }
}
