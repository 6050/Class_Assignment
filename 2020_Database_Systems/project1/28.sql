SELECT T.name AS '노멀 타입 혹은 전기 타입 포켓몬을 잡은 트레이너의 이름', AVG(level) AS '포켓몬의 평균 레벨'
FROM Trainer AS T, CatchedPokemon, Pokemon AS P
WHERE T.id = owner_id AND pid = P.id AND (type = 'Normal' OR type = 'Electric')
GROUP BY T.name
ORDER BY AVG(level);