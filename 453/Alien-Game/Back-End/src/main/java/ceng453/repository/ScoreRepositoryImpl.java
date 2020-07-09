package ceng453.repository;

import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ScoreRepositoryImpl implements CustomScoreRepository {


    @PersistenceContext
    EntityManager entityManager;

    private String[] key_array = {"username", "score"};


    @Override
    public List<Map<String, String>> getLeaderBoard() {
        return generateResponse("select U.name,  sum(S.score) as score_sum " +
                "from( user U inner join score S on U.user_id = S.user_id  )" +
                "group by U.user_id order by score_sum desc");
    }


    @Override
    public List<Map<String, String>> getLeaderBoardWeekly() {
        return generateResponse("select U.name, sum(S.score) as score_sum " +
                "from( user U inner join score S on U.user_id = S.user_id  ) " +
                "where S.date >= date_sub(now(), interval 7 day)" +
                "group by U.user_id order by score_sum desc");
    }

    private List<Map<String, String>> generateResponse(String query_string) {
        Query query = entityManager
                .createNativeQuery(query_string);
        List<Object[]> result = query.getResultList();
        List<Map<String, String>> hashmapList = new ArrayList<>();

        for (int i = 0; i < result.size(); i++) {
            hashmapList.add(new HashMap<>());
            for (int j = 0; j < 2; j++) {
                hashmapList.get(i).put(key_array[j], result.get(i)[j].toString());
            }
        }
        return hashmapList;
    }
}
