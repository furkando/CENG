package application.controller;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class MainMenuController {

    private static final String LOGIN_PAGE_URL = "/fxml/loginPage.fxml";
    private static final String LEADER_BOARD_URL = "/fxml/leaderBoard.fxml";

    /**
     * This method starts the game for given player id.
     *
     * @param event Event of button
     * @throws IOException It throws an exception if it is not load to FXML
     */
    @FXML
    protected void handlePlayButtonAction(ActionEvent event) throws IOException {
        Stage currentStage = (Stage) ((Node) event.getSource()).getScene().getWindow();
        GameController gameController = new GameController();
        gameController.playGame(currentStage, currentStage.getScene().getRoot().getId());
    }

    /**
     * This method prepares Login Page.
     *
     * @param event Event of button
     * @throws IOException It throws an exception if it is not load to FXML
     */
    @FXML
    protected void handleLogoutButtonAction(ActionEvent event) throws IOException {
        Stage currentStage = (Stage) ((Node) event.getSource()).getScene().getWindow();
        Parent loginPage = FXMLLoader.load(getClass().getResource(LOGIN_PAGE_URL));
        Scene scene = new Scene(loginPage, 600, 800);
        currentStage.setScene(scene);
        currentStage.show();
    }

    /**
     * This method prepares Leader Board page.
     *
     * @param event Event of button
     * @throws IOException It throws an exception if it is not load to FXML
     */
    @FXML
    protected void handleLeaderBoardButtonAction(ActionEvent event) throws IOException {
        Stage currentStage = (Stage) ((Node) event.getSource()).getScene().getWindow();
        Parent registerPage = FXMLLoader.load(getClass().getResource(LEADER_BOARD_URL));
        registerPage.setId(currentStage.getScene().getRoot().getId());
        Scene scene = new Scene(registerPage, 600, 800);
        currentStage.setScene(scene);
        currentStage.show();
    }
}