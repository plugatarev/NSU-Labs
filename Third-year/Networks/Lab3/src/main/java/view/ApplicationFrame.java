package view;

import modelAPI.*;
import responses.*;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;

public class ApplicationFrame {
    private static final int FRAME_WIDTH = 800;
    private static final int FRAME_HEIGHT = 800;
    private static final String DEFAULT_PATH_TO_KEYS = "src/main/resources/api_keys.property";

    private final APIKeys keys = new APIKeys(DEFAULT_PATH_TO_KEYS);
    private final ArrayList<Address.Point> geoPoints = new ArrayList<>();

    private final JFrame frame = new JFrame("GeoFinder");
    private final JTextField placeName = new JTextField();

    private final JButton findButton = new JButton("Find place");
    private final JButton infoAboutPlaceButton = new JButton("Get information");

    private final DefaultListModel<String> addressesListModel = new DefaultListModel<>();
    private final JList<String> addressesList = new JList<>(addressesListModel);

    private final JLabel weatherLabel = new JLabel();

    private final JScrollPane addressesScrollPane = new JScrollPane(addressesList);

    public ApplicationFrame() {
        frame.setSize(FRAME_WIDTH, FRAME_HEIGHT);
        frame.setResizable(false);
        frame.setLayout(new VerticalLayout());
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        addressesList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        setSizes();
        addComponents();
        addActionListeners();
    }

    private void addComponents() {
        addressesListModel.add(0, "Please, type name of place you want to know about and click \"Find!\"");
        frame.add(placeName);
        frame.add(findButton);
        frame.add(addressesScrollPane);
        frame.add(infoAboutPlaceButton);
        frame.add(weatherLabel);
    }

    private void setSizes() {
        addressesList.setPreferredSize(new Dimension(500, 200));
        placeName.setPreferredSize(new Dimension(200, 20));
        weatherLabel.setPreferredSize(new Dimension(400, 100));
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

    private void showInfoAboutInterestingPlace(List<PlaceInformation> places) {
        JTextArea textArea = new JTextArea(30, 70);
        textArea.setFont(new Font(Font.DIALOG, Font.BOLD,15));
        int i = 0;
        for (PlaceInformation info : places) {
            if (i++ > 10) break;
            textArea.append(info.toString() + "\n");
        }
        textArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(textArea);
        if (places.size() == 0) textArea.setText("No interesting places was found");
        JOptionPane.showMessageDialog(frame, scrollPane);
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

            CompletableFuture<List<CompletableFuture<PlaceInformation>>> list =
                    OpenTripMapAPI.getInterestingPlaces(lat, lng, keys.getOpenTripMapAPIKey())
                                  .thenApply(response ->
                                    response.features()
                                            .stream()
                                            .map(k -> k.properties().xid())
                                            .map(xid -> OpenTripMapAPI.getInfoAboutPlace(xid, keys.getOpenTripMapAPIKey()))
                                            .toList());
            CompletableFuture<WeatherDesc> weatherByCords = OpenWeatherMapAPI.getWeatherByCords(lat, lng, keys.getOpenWeatherAPIKey());

            CompletableFuture.allOf(weatherByCords, list).join();
            updateWeatherInfo(weatherByCords.join());
            showInfoAboutInterestingPlace(list.join().stream().map(CompletableFuture::join).toList());
        });
    }
}
