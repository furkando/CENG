package application.controller;

import application.consumer.RestServiceConsumer;
import application.entity.Item;
import application.entity.Player;
import application.entity.PlayerMixIn;
import application.helper.AlertHelper;
import ceng453.entity.User;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.ObjectMapper;
import javafx.animation.AnimationTimer;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.ButtonType;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.paint.ImagePattern;
import javafx.stage.Stage;
import javafx.stage.Window;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.util.List;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

class GameController {

    private static final String SERVER_IP = "127.0.0.1";
    private final static int SERVER_PORT = 8082;

    private static final String EASY_ALIEN_IMAGE_URL = "/images/ship1.png";
    private static final String MEDIUM_ALIEN_IMAGE_URL = "/images/ship2.png";
    private static final String HARD_ALIEN_IMAGE_URL = "/images/ship3.png";
    private static final String FINAL_ALIEN_IMAGE_URL = "/images/ship4.png";


    private static final String PLAYER_SHIP_IMAGE_URL = "/images/player1.png";
    private static final String PLAYER2_SHIP_IMAGE_URL = "/images/player2.png";


    private static final String MAIN_MENU_URL = "/fxml/mainMenu.fxml";

    private static final Integer WIDTH_OF_SCREEN = 600;
    private static final Integer HEIGHT_OF_SCREEN = 800;

    private InetAddress ip = InetAddress.getByName(SERVER_IP);

    private Socket s = new Socket(ip, SERVER_PORT);

    private DataOutputStream dos = new DataOutputStream(s.getOutputStream());
    private DataInputStream dis = new DataInputStream(s.getInputStream());


    private String userId;

    private Pane root = new Pane();

    private double timeCounter = 0;

    private Integer currentLevel = 0;

    private Integer otherUserID = 0;

    private Integer winnerID = 0;

    private Integer enemyCount = 0;

    private Integer score = 0;

    private Integer pattern = 0;

    private Stage mainStage = new Stage();

    private Item player = new Item(270, 740, 60, 60, "player", 1);

    private Item player2 = new Item(1000, 1000, 60, 60, "player", 2);

    private Item finalBoss = new Item(20, 100, 560, 240, "enemy", 0);

    private Alert alert = new Alert(Alert.AlertType.INFORMATION);


    private Image level1Image = new Image(getClass().getResourceAsStream("/images/level1.png"));
    private Image level2Image = new Image(getClass().getResourceAsStream("/images/level2.png"));
    private Image level3Image = new Image(getClass().getResourceAsStream("/images/level3.png"));

    private Label levelLabel = new Label();

    private Label scoreLabel = new Label();

    private Label bossHealthLabel = new Label();


    private Label healthLabel = new Label();

    private AnimationTimer timer = new AnimationTimer() {
        @Override
        public void handle(long now) {
            update();
        }
    };

    GameController() throws IOException {
    }

    /**
     * This method creates content for game and set Scene for the game.
     *
     * @param stage Stage of the game
     * @param id    Id of active User
     * @throws IOException It throws an exception if it is not load to FXML
     */
    void playGame(Stage stage, String id) throws IOException {
        userId = id;
        root.getChildren().remove(0, root.getChildren().size());
        currentLevel = 1;


        Thread readMessage = new Thread(() -> {

            while (true) {
                try {
                    String msg = dis.readUTF();
                    System.out.println(msg);
                    if (msg.equals("shoot")) {
                        Platform.runLater(() -> shoot(player2));
                    } else if (msg.equals("start")) {
                        dos.writeUTF("user-" + userId);
                        TimeUnit.SECONDS.sleep(3);
                        Platform.runLater(() -> {
                            timeCounter = 0;
                            timer.start();
                            alert.close();
                        });

                    } else if (msg.contains("user")) {
                        if (!(userId.equals((msg.split("-")[1])))) {
                            otherUserID = Integer.valueOf(msg.split("-")[1]);
                        }
                    } else {
                        String[] coordinateAndUserId = msg.split("-");
                        player2.setTranslateX(Double.valueOf(coordinateAndUserId[0]));
                        player2.setTranslateY(Double.valueOf(coordinateAndUserId[1]));
                    }

                } catch (IOException e) {
                    e.printStackTrace();
                    break;
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
        readMessage.start();

        Scene scene = new Scene(createContent());
        scene.setOnKeyPressed(key -> {
            Thread sendMessage = new Thread(() -> {
                if (currentLevel == 4 || currentLevel == 5) {
                    String message = "";

                    if (currentLevel == 5) {
                        message = "logout";
                    } else if (currentLevel == 4) {
                        if (key.getCode() == KeyCode.SPACE) {
                            message = "shoot";
                        } else {
                            double x = player.getTranslateX();
                            double y = player.getTranslateY();
                            message = Double.toString(x) + "-" + Double.toString(y);
                        }
                    }

                    try {
                        dos.writeUTF(message);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            });
            if (key.getCode() == KeyCode.A) {
                if (player.getTranslateX() > 0)
                    player.moveLeft();
            } else if (key.getCode() == KeyCode.W) {
                if (player.getTranslateY() > 0)
                    player.moveUp();
            } else if (key.getCode() == KeyCode.D) {
                if (player.getTranslateX() + 60 < WIDTH_OF_SCREEN)
                    player.moveRight();
            } else if (key.getCode() == KeyCode.S) {
                if (player.getTranslateY() + 60 < HEIGHT_OF_SCREEN)
                    player.moveDown();
            } else if (key.getCode() == KeyCode.SPACE) {
                shoot(player);
            }
            sendMessage.start();

        });
        stage.setOnCloseRequest(event -> {

            String closeMessage = "close-" + currentLevel.toString();

            try {
                dos.writeUTF(closeMessage);
                s.close();
                dos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            Platform.exit();
            System.exit(0);
        });
        stage.setScene(scene);
        mainStage = stage;
        stage.show();


    }

    /**
     * This method creates labels for game screen and starts timer for the game.
     *
     * @return Parent to set the scene
     * @throws IOException It throws an exception if it is not load to FXML
     */
    private Parent createContent() throws IOException {
        root.setPrefSize(600, 800);
        root.getChildren().add(player);
        Image img = new Image(PLAYER_SHIP_IMAGE_URL);
        player.setFill(new ImagePattern(img));
        levelLabel.relocate(0, 0);
        levelLabel.setGraphic(new ImageView(level1Image));

        scoreLabel.relocate(520, 30);
        scoreLabel.setText("Score : " + score.toString());

        healthLabel.relocate(220, 30);
        healthLabel.setText("Health : " + player.getHealth().toString() + "HP");

        root.getChildren().addAll(levelLabel, scoreLabel, healthLabel);


        nextLevelPreparer(currentLevel);


        timer.start();


        return root;
    }

    /**
     * This method creates game objects for given level.
     *
     * @param level Level to prepare
     * @throws IOException It throws an exception if it is not load to FXML
     */
    private void nextLevelPreparer(Integer level) throws IOException {

        if (level == 1) {
            createEasyAlien();
            enemyCount = 5;
        } else if (level == 2) {
            levelLabel.setGraphic(new ImageView(level2Image));
            createEasyAlien();
            createMediumAlien();
            enemyCount = 11;
        } else if (level == 3) {
            levelLabel.setGraphic(new ImageView(level3Image));
            createEasyAlien();
            createMediumAlien();
            createHardAlien();
            enemyCount = 16;
        } else if (level == 4) {


            player.health = 200;
            player2.health = 200;
            player2.setTranslateX(270);
            player2.setTranslateY(740);
            healthLabel.setText("Health : " + player.getHealth().toString() + "HP");
            removeBullets();
            root.getChildren().remove(levelLabel);
            createFinalAlien();
            Image img = new Image(PLAYER2_SHIP_IMAGE_URL);
            player2.setFill(new ImagePattern(img));
            root.getChildren().add(player2);
            player.setTranslateX(270);
            player.setTranslateY(740);
            bossHealthLabel.relocate(20, 30);
            bossHealthLabel.setText("Boss Health : " + finalBoss.getHealth().toString() + "HP");
            root.getChildren().add(bossHealthLabel);
            enemyCount = 1;
            timer.stop();
            Window owner = mainStage.getScene().getWindow();


            String message = "level4";

            try {
                dos.writeUTF(message);
            } catch (IOException e) {
                e.printStackTrace();
            }

            alert.setTitle("Waiting for other player!");
            alert.setHeaderText(null);
            alert.setContentText("Waiting for another player... This screen will close automatically and game will start in 3 seconds !!!");
            alert.initOwner(owner);
            alert.getDialogPane().lookupButton(ButtonType.OK).setVisible(false);
            alert.show();


        } else {
            gameOverScene();
        }


    }

    /**
     * This method creates final boss.
     */
    private void createFinalAlien() {
        finalBoss.health = 500;
        root.getChildren().add(finalBoss);
        Image img = new Image(FINAL_ALIEN_IMAGE_URL);
        finalBoss.setFill(new ImagePattern(img));
    }


    /**
     * This method creates hard aliens.
     */
    private void createHardAlien() {
        for (int i = 0; i < 5; i++) {
            Item s = new Item(75 + i * 100, 200, 50, 50, "enemy", 0);
            root.getChildren().add(s);
            Image img = new Image(HARD_ALIEN_IMAGE_URL);
            s.setFill(new ImagePattern(img));
        }
    }

    /**
     * This method creates medium aliens.
     */
    private void createMediumAlien() {
        for (int i = 0; i < 6; i++) {
            Item s = new Item(25 + i * 100, 150, 50, 50, "enemy", 0);
            root.getChildren().add(s);
            Image img = new Image(MEDIUM_ALIEN_IMAGE_URL);
            s.setFill(new ImagePattern(img));
        }
    }

    /**
     * This method creates easy aliens.
     */
    private void createEasyAlien() {
        for (int i = 0; i < 5; i++) {
            Item s = new Item(75 + i * 100, 100, 50, 50, "enemy", 0);
            root.getChildren().add(s);
            Image img = new Image(EASY_ALIEN_IMAGE_URL);
            s.setFill(new ImagePattern(img));
        }
    }

    /**
     * This method gets all game objects and return them as a list.
     *
     * @return List of items
     */
    private List<Item> items() {
        List<Object> itemList = root.getChildren().stream().filter(n -> n.getClass().getName().contains("Item")).collect(Collectors.toList());

        return itemList.stream().map(n -> (Item) n).collect(Collectors.toList());
    }

    /**
     * This method updates game screen with timer and creates random fires from aliens and also remove objects when they die.
     */
    private void update() {
        final Integer[] flag = {0};
        timeCounter += 0.016;
        items().forEach(item -> {
            if (item.type.equals("enemybullet")) {

                item.moveDownBullet();

                if (item.getBoundsInParent().intersects(player.getBoundsInParent())) {
                    player.health -= 20;
                    healthLabel.setText("Health : " + player.getHealth().toString() + "HP");
                    if (player.health <= 0) {
                        player.dead = true;
                        try {
                            removeItems();
                            gameOverScene();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                    item.dead = true;
                }
                if (item.getBoundsInParent().intersects(player2.getBoundsInParent())) {
                    player2.health -= 20;
                    if (player2.health <= 0) {
                        player2.dead = true;
                        try {
                            gameOverScene();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                    item.dead = true;
                }
                if (item.getTranslateY() > 800) {
                    item.dead = true;
                }
            } else if (item.type.equals("playerbullet")) {
                item.moveUp();

                items().stream().filter(e -> e.type.equals("enemy")).forEach(enemy -> {
                    if (item.getBoundsInParent().intersects(enemy.getBoundsInParent())) {
                        if (currentLevel == 4) {
                            finalBoss.health -= 20;
                            item.dead = true;
                            bossHealthLabel.setText("Boss Health : " + finalBoss.getHealth().toString() + "HP");
                            if (finalBoss.health <= 0) {
                                if (item.player == 1) {
                                    winnerID = Integer.valueOf(userId);
                                } else if (item.player == 2) {
                                    winnerID = otherUserID;
                                }
                                enemyCount--;
                                finalBoss.dead = true;
                                removeItems();
                                nextLevel();
                            }
                        } else {
                            enemy.dead = true;
                            item.dead = true;
                            enemyCount--;
                            score += 10;
                            scoreLabel.setText("Score : " + score.toString());
                            removeItems();
                            nextLevel();
                        }
                    }
                    if (item.getTranslateY() < 0) {
                        item.dead = true;
                    }
                });

            } else if (item.type.equals("player")) {

                items().stream().filter(e -> e.type.equals("enemy")).forEach(enemy -> {
                    if (item.getBoundsInParent().intersects(enemy.getBoundsInParent())) {
                        enemy.dead = true;
                        enemyCount--;
                        score += 10;
                        scoreLabel.setText("Score : " + score.toString());
                        removeItems();
                        nextLevel();
                    }
                });

            } else if (item.type.equals("enemy")) {
                if (currentLevel == 4) {
                    if (timeCounter > 1) {
                        bossShoot();
                    }
                } else {
                    if (timeCounter > 1) {
                        if (Math.random() < 0.5) {
                            shoot(item);
                        }
                    }
                }


            }

        });

        removeItems();

        if (timeCounter > 1) {
            timeCounter = 0;
        }

    }

    /**
     * This method removes all items from scene
     */
    private void removeItems() {
        root.getChildren().

                removeIf(n ->
                {
                    if (n.getClass().getName().contains("Item")) {
                        Item s = (Item) n;
                        return s.dead;
                    } else {
                        return false;
                    }
                });
    }

    /**
     * This method removes remaining bullets on the screen
     */
    private void removeBullets() {
        root.getChildren().

                removeIf(n ->
                {
                    if (n.getClass().getName().contains("Item")) {
                        Item s = (Item) n;
                        return s.type.equals("enemybullet");
                    } else {
                        return false;
                    }
                });
    }

    /**
     * This method gets next level.
     */
    private void nextLevel() {
        if (enemyCount == 0) {
            currentLevel++;
            try {
                nextLevelPreparer(currentLevel);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * This method creates fires for boss with a pattern which we created
     */
    private void bossShoot() {
        Item s1 = new Item((int) finalBoss.getTranslateX() + 40, (int) finalBoss.getTranslateY() + 160, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s2 = new Item((int) finalBoss.getTranslateX() + 80, (int) finalBoss.getTranslateY() + 80, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s3 = new Item((int) finalBoss.getTranslateX() + 120, (int) finalBoss.getTranslateY() + 160, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s4 = new Item((int) finalBoss.getTranslateX() + 160, (int) finalBoss.getTranslateY() + 240, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s5 = new Item((int) finalBoss.getTranslateX() + 200, (int) finalBoss.getTranslateY() + 80, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s6 = new Item((int) finalBoss.getTranslateX() + 240, (int) finalBoss.getTranslateY() + 240, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s7 = new Item((int) finalBoss.getTranslateX() + 280, (int) finalBoss.getTranslateY() + 160, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s8 = new Item((int) finalBoss.getTranslateX() + 320, (int) finalBoss.getTranslateY() + 240, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s9 = new Item((int) finalBoss.getTranslateX() + 360, (int) finalBoss.getTranslateY() + 80, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s10 = new Item((int) finalBoss.getTranslateX() + 400, (int) finalBoss.getTranslateY() + 240, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s11 = new Item((int) finalBoss.getTranslateX() + 440, (int) finalBoss.getTranslateY() + 160, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s12 = new Item((int) finalBoss.getTranslateX() + 480, (int) finalBoss.getTranslateY() + 80, 5, 10, finalBoss.type + "bullet", Color.BLACK);
        Item s13 = new Item((int) finalBoss.getTranslateX() + 520, (int) finalBoss.getTranslateY() + 160, 5, 10, finalBoss.type + "bullet", Color.BLACK);

        if ((pattern % 8) == 0) {
            root.getChildren().addAll(s2, s4, s6, s8, s9, s13);
            pattern++;
        } else if ((pattern % 8) == 1) {
            root.getChildren().addAll(s1, s3, s8, s9, s10);
            pattern++;
        } else if ((pattern % 8) == 2) {
            root.getChildren().addAll(s2, s3, s8, s9, s10, s11, s13);
            pattern++;
        } else if ((pattern % 8) == 3) {
            root.getChildren().addAll(s1, s7, s8, s10, s11, s12);
            pattern++;
        } else if ((pattern % 8) == 4) {
            root.getChildren().addAll(s2, s5, s6, s7, s12, s13);
            pattern++;
        } else if ((pattern % 8) == 5) {
            root.getChildren().addAll(s1, s3, s6, s9, s10, s12);
            pattern++;
        } else if ((pattern % 8) == 6) {
            root.getChildren().addAll(s3, s6, s7, s11, s13);
            pattern++;
        } else if ((pattern % 8) == 7) {
            root.getChildren().addAll(s2, s5, s6, s8, s12);
            pattern = 0;
        }


    }

    /**
     * This method creates fires for player and also for aliens.
     *
     * @param item Item to create bullet
     */
    private void shoot(Item item) {
        if (item.type.equals("player")) {

            Item s = new Item((int) item.getTranslateX() + 30, (int) item.getTranslateY() + 30, 5, 10, item.type + "bullet", Color.ORANGE);
            if (item.getPlayer() == 1) {
                s.player = 1;
            } else if (item.getPlayer() == 2) {
                s.player = 2;
            }
            root.getChildren().add(s);

        } else {
            Item s = new Item((int) item.getTranslateX() + 25, (int) item.getTranslateY() + 25, 5, 10, item.type + "bullet", Color.PURPLE);
            root.getChildren().add(s);

        }

    }

    /**
     * This method prepares game over screen and load Main Menu.
     *
     * @throws IOException It throws an exception if it is not load to FXML
     */
    private void gameOverScene() throws IOException {
        timer.stop();

        if (currentLevel < 4) {
            String message = "close-" + currentLevel.toString();

            try {
                dos.writeUTF(message);
            } catch (IOException e) {
                e.printStackTrace();
            }

            Window owner = mainStage.getScene().getWindow();
            AlertHelper.showAlert(Alert.AlertType.INFORMATION, owner, "Game is Over!",
                    "Your score is " + score + "!");
        } else {

            String message = "logout";

            try {
                dos.writeUTF(message);
            } catch (IOException e) {
                e.printStackTrace();
            }


            Window owner = mainStage.getScene().getWindow();

            RestServiceConsumer restServiceConsumer = new RestServiceConsumer();
            restServiceConsumer.addScore(userId, score.toString());
            ObjectMapper mapper = new ObjectMapper();
            mapper.enable(JsonParser.Feature.ALLOW_UNQUOTED_FIELD_NAMES);
            mapper.addMixIn(Player.class, PlayerMixIn.class);
            User winner2 = mapper.readValue(restServiceConsumer.getUser(userId), User.class);
            User winner3 = mapper.readValue(restServiceConsumer.getUser(otherUserID.toString()), User.class);

            if (player.dead) {
                AlertHelper.showAlert(Alert.AlertType.INFORMATION, owner, "Game is Over!",
                        "Winner is " + winner3.getName() + "!");
            } else if (player2.dead) {
                AlertHelper.showAlert(Alert.AlertType.INFORMATION, owner, "Game is Over!",
                        "Winner is " + winner2.getName() + "!");
            } else {
                User winner = mapper.readValue(restServiceConsumer.getUser(winnerID.toString()), User.class);
                AlertHelper.showAlert(Alert.AlertType.INFORMATION, owner, "Game is Over!",
                        "Winner is " + winner.getName() + "!");
            }

        }

        s.close();
        dos.close();
        Parent mainMenu = FXMLLoader.load(getClass().getResource(MAIN_MENU_URL));

        mainMenu.setId(userId);

        Scene mainMenuScene = new Scene(mainMenu);

        mainStage.setScene(mainMenuScene);

        mainStage.show();


    }

}