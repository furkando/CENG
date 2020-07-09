package application.helper;


import javafx.scene.control.Alert;
import javafx.stage.Window;


public class AlertHelper {
    /**
     * This method creates alerts with the given title and message.
     *
     * @param alertType Alert type
     * @param owner Window owner
     * @param title Title to set
     * @param message Message to show
     */
    public static void showAlert(Alert.AlertType alertType, Window owner, String title, String message) {
        Alert alert = new Alert(alertType);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.initOwner(owner);
        alert.show();
    }
}