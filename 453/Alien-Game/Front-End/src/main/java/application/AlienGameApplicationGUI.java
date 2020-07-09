package application;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class AlienGameApplicationGUI extends Application {

    private static final String LOGIN_PAGE_URL = "/fxml/loginPage.fxml";

    /**
     * This method loads Login Page and set Scene to that page.
     *
     * @param stage Stage to set scene
     * @throws IOException It throws an exception if it is not load to FXML
     */
    private void mainMenuScene(Stage stage) throws IOException {

        Parent mainMenu = FXMLLoader.load(getClass().getResource(LOGIN_PAGE_URL));

        Scene mainMenuScene = new Scene(mainMenu);

        stage.setScene(mainMenuScene);

        stage.show();

    }

    /**
     * This method starts the application.
     *
     * @param stage Stage
     * @throws Exception Exception message
     */
    @Override
    public void start(Stage stage) throws Exception {

        stage.setTitle("Dünyayı Kurtaran Adamın Oğlu ve Uzay Gemisi");

        mainMenuScene(stage);


    }


    public static void main(String[] args) {
        launch(args);
    }
}