package application.consumer;

import application.entity.Player;
import application.entity.PlayerMixIn;
import ceng453.entity.User;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class RestServiceConsumer {

    private static final String REST_URL = "http://127.0.0.1:8080/";

    public RestServiceConsumer() {
    }

    /**
     * This method connects the rest service and post request to save the user.
     *
     * @param user User to register
     * @return Output from given connection
     */
    public String register(User user) {
        try {
            String registerUrl = REST_URL.concat("users");
            URL url = new URL(registerUrl);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setDoOutput(true);
            conn.setRequestMethod("POST");
            conn.setRequestProperty("Content-Type", "application/json");

            String name = user.getName();
            String email = user.getEmail();
            String password = user.getPassword();

            String input = "{\"name\":\"";

            input = input.concat(name);
            input = input.concat("\",\"password\":\"");
            input = input.concat(password);
            input = input.concat("\",\"email\":\"");
            input = input.concat(email);
            input = input.concat("\"}");


            OutputStream os = conn.getOutputStream();
            os.write(input.getBytes());
            os.flush();

            if (conn.getResponseCode() != HttpURLConnection.HTTP_OK) {
                throw new RuntimeException("Failed : HTTP error code : "
                        + conn.getResponseCode());
            }

            BufferedReader br = new BufferedReader(new InputStreamReader(
                    (conn.getInputStream())));

            String output;
            output = br.readLine();

            conn.disconnect();

            return output;

        } catch (IOException e) {

            e.printStackTrace();
            return "Fail";

        }

    }

    /**
     * This method post request to rest service and return the result as an user.
     *
     * @param user User to login
     * @return User
     */
    public User login(User user) {
        try {
            String loginUrl = REST_URL.concat("login");
            URL url = new URL(loginUrl);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setDoOutput(true);
            conn.setRequestMethod("POST");
            conn.setRequestProperty("Content-Type", "application/json");

            String name = user.getName();
            String password = user.getPassword();

            String input = "{\"name\":\"";
            input = input.concat(name);
            input = input.concat("\",\"password\":\"");
            input = input.concat(password);
            input = input.concat("\"}");


            OutputStream os = conn.getOutputStream();
            os.write(input.getBytes());
            os.flush();

            if (conn.getResponseCode() != HttpURLConnection.HTTP_OK) {
                throw new RuntimeException("Failed : HTTP error code : "
                        + conn.getResponseCode());
            }

            BufferedReader br = new BufferedReader(new InputStreamReader(
                    (conn.getInputStream())));


            String output;
            String loginUser = "";

            while ((output = br.readLine()) != null) {
                loginUser = loginUser.concat(output);
            }

            if (loginUser.equals("")) {
                conn.disconnect();
                return null;
            }

            ObjectMapper mapper = new ObjectMapper();
            mapper.enable(JsonParser.Feature.ALLOW_UNQUOTED_FIELD_NAMES);
            mapper.addMixIn(Player.class, PlayerMixIn.class);
            mapper.configure(DeserializationFeature.ACCEPT_SINGLE_VALUE_AS_ARRAY, true);

            conn.disconnect();

            return mapper.readValue(loginUser, User.class);

        } catch (IOException e) {

            e.printStackTrace();
            return null;
        }
    }

    /**
     * This method takes score and user id and post request the rest service the save that score to user with given id.
     *
     * @param userId User id to set score
     * @param score  Score to set
     */
    public void addScore(String userId, String score) {
        try {

            String scoreUrl = REST_URL + "users/" + userId + "/" + score;
            URL url = new URL(scoreUrl);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setDoOutput(true);
            conn.setRequestMethod("POST");
            conn.setRequestProperty("Content-Type", "application/json");


            if (conn.getResponseCode() != HttpURLConnection.HTTP_OK) {
                throw new RuntimeException("Failed : HTTP error code : "
                        + conn.getResponseCode());
            }


            conn.disconnect();


        } catch (IOException e) {

            e.printStackTrace();

        }

    }

    /**
     * This method gets leader board from rest service and return as an Json.
     *
     * @param time Time to get leader board
     * @return Leader board
     */

    public String getScoreboard(String time) {
        try {
            String leaderBoardURL = REST_URL + "scores/";
            if (time.equals("weekly")) {
                leaderBoardURL = leaderBoardURL.concat("weekly");
            } else {
                leaderBoardURL = leaderBoardURL.concat("all");
            }
            URL url = new URL(leaderBoardURL);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setDoOutput(true);
            conn.setRequestMethod("GET");
            conn.setRequestProperty("Content-Type", "application/json");

            if (conn.getResponseCode() != 200) {
                throw new RuntimeException("Failed : HTTP error code : "
                        + conn.getResponseCode());
            }

            BufferedReader br = new BufferedReader(new InputStreamReader(
                    (conn.getInputStream())));

            String output;
            String scoreBoard = "";
            while ((output = br.readLine()) != null) {
                scoreBoard = scoreBoard.concat(output);
            }

            conn.disconnect();

            return scoreBoard;

        } catch (IOException e) {

            e.printStackTrace();

            return null;

        }
    }

    public String getUser(String userId) {
        try {
            String userUrl = REST_URL + "users/" + userId ;
            URL url = new URL(userUrl);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setDoOutput(true);
            conn.setRequestMethod("GET");
            conn.setRequestProperty("Content-Type", "application/json");

            if (conn.getResponseCode() != 200) {
                throw new RuntimeException("Failed : HTTP error code : "
                        + conn.getResponseCode());
            }

            BufferedReader br = new BufferedReader(new InputStreamReader(
                    (conn.getInputStream())));

            String output;
            String userName = "";
            while ((output = br.readLine()) != null) {
                userName = userName.concat(output);
            }

            conn.disconnect();

            return userName;

        } catch (IOException e) {

            e.printStackTrace();

            return null;

        }
    }
}
