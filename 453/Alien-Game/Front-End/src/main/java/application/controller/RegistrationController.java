package application.controller;

import application.consumer.RestServiceConsumer;
import application.helper.AlertHelper;
import ceng453.entity.User;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import javafx.stage.Window;

import javax.mail.internet.InternetAddress;
import java.io.IOException;

public class RegistrationController {

    private static final String LOGIN_PAGE_URL = "/fxml/loginPage.fxml";

    @FXML
    private TextField nameField;

    @FXML
    private TextField emailField;

    @FXML
    private PasswordField passwordField;

    @FXML
    private Button submitButton;

    /**
     * This method checks the given information and save the user to database.
     *
     * @param event Event of button
     * @throws IOException It throws an exception if it is not load to FXML
     */
    @FXML
    protected void handleSubmitButtonAction(ActionEvent event) throws IOException {
        Window owner = submitButton.getScene().getWindow();
        if (nameField.getText().isEmpty()) {
            AlertHelper.showAlert(Alert.AlertType.ERROR, owner, "Form Error!",
                    "Please enter your name");
            return;
        }
        if (emailField.getText().isEmpty()) {
            AlertHelper.showAlert(Alert.AlertType.ERROR, owner, "Form Error!",
                    "Please enter your email id");
            return;
        }

        if (!isValidEmailAddress(emailField.getText())) {
            AlertHelper.showAlert(Alert.AlertType.ERROR, owner, "Form Error!",
                    "Please provide a valid Email");
            return;
        }
        if (passwordField.getText().isEmpty()) {
            AlertHelper.showAlert(Alert.AlertType.ERROR, owner, "Form Error!",
                    "Please enter a password");
            return;
        }

        if (passwordField.getText().length() < 5) {
            AlertHelper.showAlert(Alert.AlertType.ERROR, owner, "Form Error!",
                    "Your password must have at least 5 characters");
            return;
        }

        User user = User.builder().name(nameField.getText()).password(passwordField.getText()).email(emailField.getText()).build();

        RestServiceConsumer restServiceConsumer = new RestServiceConsumer();
        String output = restServiceConsumer.register(user);
        if (output.equals("Username Already Exists!")) {
            AlertHelper.showAlert(Alert.AlertType.ERROR, owner, "Form Error!",
                    "Username Already Exists!");
            return;
        }
        if (output.equals("User Added")) {
            AlertHelper.showAlert(Alert.AlertType.INFORMATION, owner, "Confirmed!",
                    "Registration is Succesfull!");
            Stage currentStage = (Stage) ((Node) event.getSource()).getScene().getWindow();
            Parent loginPage = FXMLLoader.load(getClass().getResource(LOGIN_PAGE_URL));
            Scene scene = new Scene(loginPage, 600, 800);
            currentStage.setScene(scene);
            currentStage.show();
        }

    }

    /**
     * This method prepares Login Page.
     *
     * @param event Event of button
     * @throws IOException It throws an exception if it is not load to FXML
     */
    @FXML
    protected void handleCancelButtonAction(ActionEvent event) throws IOException {

        Stage currentStage = (Stage) ((Node) event.getSource()).getScene().getWindow();
        Parent loginPage = FXMLLoader.load(getClass().getResource(LOGIN_PAGE_URL));
        Scene scene = new Scene(loginPage, 600, 800);
        currentStage.setScene(scene);
        currentStage.show();

    }

    /**
     * This method check if given email address valid or not.
     *
     * @param email Email to check
     * @return True if email is valid,otherwise false
     */
    private static boolean isValidEmailAddress(String email) {
        boolean result = true;
        try {
            InternetAddress emailAddr = new InternetAddress(email);
            emailAddr.validate();
        } catch (javax.mail.internet.AddressException ex) {
            result = false;
        }
        return result;
    }
}