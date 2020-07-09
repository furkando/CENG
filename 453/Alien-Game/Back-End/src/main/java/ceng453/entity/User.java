package ceng453.entity;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.hibernate.validator.constraints.Length;

import javax.persistence.*;
import javax.validation.constraints.Email;
import javax.validation.constraints.NotNull;
import java.util.ArrayList;
import java.util.List;

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
@Entity
@Table(name = "user")
public class User {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "user_id")
    private int id;

    @Column(name = "name")
    @NotNull(message = "*Please provide your name")
    private String name;

    @Column(name = "password")
    @Length(min = 5, message = "*Your password must have at least 5 characters")
    @NotNull(message = "*Please provide your password")
    private String password;

    @Email(message = "*Please provide a valid Email")
    @NotNull(message = "*Please provide an email")
    private String email;


    @OneToMany
    @JoinColumn(name = "user_id")
    private List<Score> scoreList = new ArrayList<>();

}
