package ceng453;


import ceng453.entity.Score;
import ceng453.entity.User;
import ceng453.repository.ScoreRepository;
import ceng453.repository.UserRepository;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;

import static junit.framework.TestCase.assertEquals;
import static junit.framework.TestCase.assertNotNull;

@RunWith(SpringRunner.class)
@DataJpaTest
public class UserControllerTest {

    @Autowired
    private UserRepository userRepository;

    @Autowired
    private ScoreRepository scoreRepository;

    @Test
    public void findAll() {

        User furkan = User.builder().name("furkan1").email("furkan@a").password("asdfg").build();
        userRepository.save(furkan);
        User yemre = User.builder().name("yemre1").email("yemre@a").password("asdfg").build();
        userRepository.save(yemre);

        List<User> userList = Arrays.asList(furkan, yemre);

        assertNotNull(userRepository.findAll());

        assertEquals(userList, userRepository.findAll());
    }

    @Test
    public void getUserbyId() {

        User furkan = User.builder().name("furkan1").email("furkan@a").password("asdfg").build();
        userRepository.save(furkan);
        User yemre = User.builder().name("yemre1").email("yemre@a").password("asdfg").build();
        userRepository.save(yemre);


        assertNotNull(userRepository.findById(yemre.getId()));
        assertEquals(yemre, userRepository.findById(yemre.getId()).get());
    }

    @Test
    public void getLeaderBoard() {


        Date today = new Date();
        Score score1 = Score.builder().score(10).date(today).build();
        Score score2 = Score.builder().score(20).date(today).build();
        Score score3 = Score.builder().score(30).date(today).build();
        Score score4 = Score.builder().score(40).date(today).build();

        scoreRepository.save(score1);
        scoreRepository.save(score2);
        scoreRepository.save(score3);
        scoreRepository.save(score4);

        List<Score> scoreList1 = new ArrayList<>();
        List<Score> scoreList2 = new ArrayList<>();
        scoreList1.add(score1);
        scoreList1.add(score2);
        scoreList2.add(score3);
        scoreList2.add(score4);

        User furkan = User.builder().name("furkan1").email("furkan@a").password("asdfg").scoreList(scoreList1).build();
        userRepository.save(furkan);
        User yemre = User.builder().name("yemre1").email("yemre@a").password("asdfg").scoreList(scoreList2).build();
        userRepository.save(yemre);

        assertEquals("70", scoreRepository.getLeaderBoard().get(0).values().toArray()[0]);

        assertEquals("yemre1", scoreRepository.getLeaderBoard().get(0).values().toArray()[1]);

        assertEquals("30", scoreRepository.getLeaderBoard().get(1).values().toArray()[0]);

        assertEquals("furkan1", scoreRepository.getLeaderBoard().get(1).values().toArray()[1]);

    }

}
