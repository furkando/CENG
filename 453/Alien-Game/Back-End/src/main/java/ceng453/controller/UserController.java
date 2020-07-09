package ceng453.controller;


import ceng453.entity.Score;
import ceng453.entity.User;
import ceng453.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;

@RestController
public class UserController {


    @Autowired
    private UserService userService;


    @GetMapping("/users")
    public List<User> getUsers() {
        return userService.getAllUsers();
    }

    @GetMapping("/users/{id}")
    public User getUser(@PathVariable Integer id) {
        return userService.getUser(id);
    }

    @PostMapping("/users")
    public String addUser(@RequestBody User user) {
        return userService.addUser(user);
    }

    @PutMapping("/users/{id}")
    public void updateUser(@RequestBody User user, @PathVariable Integer id) {
        userService.updateUser(user, id);
    }

    @PostMapping("/users/{id}/{score}")
    public void addScore(@PathVariable Integer id, @PathVariable Integer score) {
        userService.addScore(id, score);
    }

    @GetMapping("/users/{id}/score")
    public List<Score> getUsersScore(@PathVariable Integer id) {
        return userService.getUsersScore(id);
    }

    @GetMapping("/scores/all")
    public List<Map<String, String>> getScores() {
        return userService.getScores();
    }

    @GetMapping("/scores/weekly")
    public List<Map<String, String>> getWeeklyScores() {
        return userService.getWeeklyScores();
    }

    @DeleteMapping("/users/{id}")
    public void deleteUser(@PathVariable Integer id) {
        userService.deleteUser(id);
    }

    @PostMapping("/login")
    public User login(@RequestBody User user) {
        return userService.login(user);
    }

}