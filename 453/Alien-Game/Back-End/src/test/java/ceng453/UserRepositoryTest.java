package ceng453;


import ceng453.entity.User;
import ceng453.repository.UserRepository;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest;
import org.springframework.test.context.junit4.SpringRunner;

import static junit.framework.TestCase.assertEquals;
import static junit.framework.TestCase.assertNotNull;

@RunWith(SpringRunner.class)
@DataJpaTest
public class UserRepositoryTest {

    @Autowired
    private UserRepository userRepository;

    @Test
    public void findByEmail() {

        User furkan = User.builder().name("huseyin").email("furkan@a").password("asdfg").build();
        userRepository.save(furkan);


        assertNotNull(userRepository.findByEmail("furkan@a"));
        assertEquals(furkan, userRepository.findByEmail("furkan@a"));
    }
}
