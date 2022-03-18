//<editor-fold desc="import {...} from '@spatial/core';">
const updateComponents = (entity) => (components) => {
    return {
        namespace: "scene",
        type: "update-components",
        payload: {
            entity,
            components,
        },
    };
};

const log = (message) => {
    return {
        namespace: "editor",
        type: "log",
        payload: {
            message,
        },
    };
};

const InputType = Object.freeze({
    FloatRange: 'FloatRange',
    String: 'String',
});
//</editor-fold>

const onUpdateEntity = ({ entity, delta, components }) => {
    const { velocity } = components.test[entity];
    const transform = components.transform[entity];
    transform.position.x += velocity * delta;

    return [
        log(`A log message from entity ${entity}`),
        updateComponents(entity)({
            transform
        }),
    ];
};

export default {
    name: "test",
    props: {
        velocity: {
            type: InputType.FloatRange,
            default: 5.0,
            min: .0,
            max: 100.0,
        },
        name: {
            type: InputType.String,
            default: "Luiz Gabriel",
        },
    },
    onUpdateEntity,
};