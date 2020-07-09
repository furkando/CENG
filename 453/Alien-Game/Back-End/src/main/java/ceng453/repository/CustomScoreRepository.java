package ceng453.repository;

import java.util.List;
import java.util.Map;

public interface CustomScoreRepository {
    List<Map<String, String>> getLeaderBoardWeekly();

    List<Map<String, String>> getLeaderBoard();


}
