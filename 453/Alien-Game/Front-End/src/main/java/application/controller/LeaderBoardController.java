package application.controller;

import application.consumer.RestServiceConsumer;
import application.entity.Player;
import application.entity.PlayerMixIn;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.stage.Stage;

import java.io.IOException;
import java.net.URL;
import java.util.List;
import java.util.ResourceBundle;

public class LeaderBoardController implements Initializable {

    private static final String MAIN_MENU_URL = "/fxml/mainMenu.fxml";

    @FXML
    private TableView<Player> weeklyTableView;
    @FXML
    private TableView<Player> allTimeTableView;
    @FXML
    private TableColumn<Player, String> weeklyUserColumn;
    @FXML
    private TableColumn<Player, String> weeklyScoreColumn;
    @FXML
    private TableColumn<Player, String> allTimeUserColumn;
    @FXML
    private TableColumn<Player, String> allTimeScoreColumn;

    /**
     * This method initialize the leader boards.
     *
     * @param location  URL
     * @param resources Resource Bundle
     */
    @Override
    public void initialize(URL location, ResourceBundle resources) {
        weeklyUserColumn.setCellValueFactory(new PropertyValueFactory<>("username"));
        weeklyScoreColumn.setCellValueFactory(new PropertyValueFactory<>("score"));
        allTimeUserColumn.setCellValueFactory(new PropertyValueFactory<>("username"));
        allTimeScoreColumn.setCellValueFactory(new PropertyValueFactory<>("score"));
        try {
            weeklyTableView.getItems().setAll(parseLeaderBoardList("weekly"));
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            allTimeTableView.getItems().setAll(parseLeaderBoardList("all"));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * This method prepares the Main Menu screen and redirect there.
     *
     * @param event Event of button
     * @throws IOException It throws an exception if it is not load to FXML
     */
    @FXML
    protected void handleMainMenuButtonAction(ActionEvent event) throws IOException {

        Stage currentStage = (Stage) ((Node) event.getSource()).getScene().getWindow();
        Parent mainMenu = FXMLLoader.load(getClass().getResource(MAIN_MENU_URL));
        mainMenu.setId(currentStage.getScene().getRoot().getId());
        Scene scene = new Scene(mainMenu, 600, 800);
        currentStage.setScene(scene);
        currentStage.show();
    }

    /**
     * This method gets time and prepares the leader board.
     *
     * @param time Leader board of given time
     * @return List of players
     * @throws IOException It throws an exception if it is not load to FXML
     */
    private List<Player> parseLeaderBoardList(String time) throws IOException {
        RestServiceConsumer restServiceConsumer = new RestServiceConsumer();
        String input = restServiceConsumer.getScoreboard(time);
        ObjectMapper mapper = new ObjectMapper();
        mapper.enable(JsonParser.Feature.ALLOW_UNQUOTED_FIELD_NAMES);
        mapper.addMixIn(Player.class, PlayerMixIn.class);
        mapper.configure(DeserializationFeature.ACCEPT_SINGLE_VALUE_AS_ARRAY, true);

        return mapper.readValue(input, new TypeReference<List<Player>>() {
        });
    }

}