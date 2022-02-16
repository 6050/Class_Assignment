SELECT name AS '가장 많은 두 타입 중 하나를 가지는 포켓몬의 이름'
FROM Pokemon
WHERE type IN(SELECT type
              FROM Pokemon
              GROUP BY type
              HAVING COUNT(*) >= ALL(SELECT COUNT(*)
                                     FROM Pokemon
                                     GROUP BY type
                                     HAVING COUNT(*) <> (SELECT MAX(count)
                                                         FROM(SELECT COUNT(*) AS count
                                                              FROM Pokemon
                                                              GROUP BY type) AS type_count)
                                     )
              )
ORDER BY name;